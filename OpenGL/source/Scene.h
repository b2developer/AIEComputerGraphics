#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shader.h"
#include "Texture.h"

#include "RenderType.h"

using namespace glm;
using namespace std;
using namespace aie;

//forward declarations
class GameObject;
class Camera;
class Light;
class Renderable;

/*
* class Scene
*
* holds a collection of GameObjects that are
* updated every frame, each GameObject's life
* depends on the scene's lifetime
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class Scene
{
public:

	float time = 0.0f;

	//list of gameObjects in the scene
	vector<GameObject*> gameObjects;

	/*
	* Scene()
	* default constructor
	*/
	Scene() {}

	/*
	* ~Scene()
	* destructor, removes all gameObjects
	*/
	~Scene();

	/*
	* update
	*
	* called once per frame
	* updates all gameObjects
	*
	* @param float deltaTime - the amount of passed since the last frame
	* @returns void
	*/
	void update(float deltaTime);

	/*
	* draw
	*
	* called once per frame after the update loop
	* draws all gameObjects
	*
	* @param GameObject* camera - pointer to the camera
	* @param ERenderType renderType - the type of pass to perform on the fragment shader
	* @returns void
	*/
	void draw(GameObject* camera, ERenderType renderType);

	/*
	* sortTransparentObjects
	*
	* sorts the transparent objects by their distance to the current rendering camera
	*
	* @param Camera* camera - camera component that is rendering the scene
	* @returns void
	*/
	void sortTransparentObjects(Camera* camera);

	/*
	* sortComponents
	*
	* sorts all components into lists 
	* to optimise the rendering process
	*
	* @returns void
	*/
	void sortComponents();

private:

	//sub-lists to sort to
	vector<Renderable*> meshes;
	vector<Renderable*> transparentMeshes;
	vector<Renderable*> sortedTransparentMeshes;
	vector<Renderable*> renderMeshes;
	vector<Light*> lights;

};