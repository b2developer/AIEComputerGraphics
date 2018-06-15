#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

#include "Renderable.h"
#include "RenderMesh.h"
#include "Light.h"

#include <gl_core_4_4.h>

#include "ShaderLibrary.h"

//destructor
Scene::~Scene()
{
	//iterate through all gameObjects, removing each
	for (vector<GameObject*>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		delete *iter;
	}
}

//main update loop
void Scene::update(float deltaTime)
{
	time += deltaTime;

	//iterate through all gameObjects, updating each
	for (vector<GameObject*>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		(*iter)->update(deltaTime);
	}
}

//main render loop
void Scene::draw(GameObject* camera, ERenderType renderType)
{
	Camera* c = (Camera*)camera->components[1];

	//render to the 5 buffers (albedo, position, normal, specular, specular highlight)
	if (renderType == ERenderType::G_PASS)
	{
		//apply the gpass to all meshes
		for (vector<Renderable*>::iterator iter = meshes.begin(); iter != meshes.end(); iter++)
		{
			(*iter)->draw(c, renderType);
		}
	}

	//render to the 2 buffers (light, specular)
	if (renderType == ERenderType::LIGHTING_PASS)
	{

		//combine multiple lighting passes
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_ONE, GL_ONE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		//apply the lpass to all lights
		for (vector<Light*>::iterator iter = lights.begin(); iter != lights.end(); iter++)
		{
			(*iter)->draw(c, renderType);
		}

		//reverse the gl option changes for lighting passes
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}

	//render to the composite pass and then render post-processing quads
	if (renderType == ERenderType::COMPOSITE_PASS || renderType == ERenderType::POST_PROCESSING_PASS)
	{
		//apply the composite pass and render pass to all render-meshes
		for (vector<Renderable*>::iterator iter = renderMeshes.begin(); iter != renderMeshes.end(); iter++)
		{
			(*iter)->draw(c, renderType);
		}
	}
}

//sorts all gameobject's components into sub-lists
void Scene::sortComponents()
{
	meshes.clear();
	renderMeshes.clear();
	lights.clear();

	//iterate through all gameObjects and their components
	for (vector<GameObject*>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		for (vector<Component*>::iterator compIter = (*iter)->components.begin(); compIter != (*iter)->components.end(); compIter++)
		{
			Renderable* renderTest = dynamic_cast<Renderable*>(*compIter);
			RenderMesh* rmTest = dynamic_cast<RenderMesh*>(*compIter);
			Light* lightTest = dynamic_cast<Light*>(*compIter);
			
			//the component is a renderable object
			if (renderTest != nullptr)
			{
				//the component is a mesh
				if (rmTest == nullptr)
				{
					meshes.push_back(renderTest);
				}
				//the component is a render mesh
				else
				{
					renderMeshes.push_back(rmTest);
				}
			}
			//the component is a light
			else if (lightTest != nullptr)
			{
				lights.push_back(lightTest);
			}
		}
	}
}
