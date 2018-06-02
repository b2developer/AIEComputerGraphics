#include "Scene.h"
#include "GameObject.h"

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
	//iterate through all gameObjects, updating each
	for (vector<GameObject*>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		(*iter)->update(deltaTime);
	}
}

//main render loop
void Scene::draw(mat4 viewProjectionMatrix)
{
	//iterate through all gameObjects, drawing each
	for (vector<GameObject*>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		(*iter)->draw(viewProjectionMatrix);
	}
}
