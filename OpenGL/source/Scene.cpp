#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

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

	mat4 viewProjectionMatrix = c->projectionMatrix * c->gameObject->transform->translationMatrix;

	SHL->lPassPipe.bind();

	vec3 lightDirection = vec3(1, -cosf(time * 2.0f), 1);

	SHL->lPassPipe.bindUniform("lightDirection", lightDirection);
	SHL->lPassPipe.bindUniform("lightDiffuse", vec4(1,1,1,1));

	//iterate through all gameObjects, drawing each
	for (vector<GameObject*>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		(*iter)->draw(viewProjectionMatrix, renderType);
	}
}
