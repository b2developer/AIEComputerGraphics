#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

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
	
	//combine multiple lighting passes
	if (renderType == ERenderType::LIGHTING_PASS)
	{
		//permenant settings
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_ONE, GL_ONE);

		//need to be enabled / disabled every frame
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND); 
	}

	//iterate through all gameObjects, drawing each
	for (vector<GameObject*>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		(*iter)->draw(c, renderType);
	}

	//reverse the gl option changes for lighting passes
	if (renderType == ERenderType::LIGHTING_PASS)
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
}
