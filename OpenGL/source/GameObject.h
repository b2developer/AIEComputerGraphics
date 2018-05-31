#pragma once
#include <vector>

using namespace std;

//forward declaration
class Component;
class Transform;

/*
* class GameObject
*
* an actor in the scene, one update is run for each game-object per frame
* which automatically calls the update function on all active components
* 
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
a
*/
class GameObject
{
public:

	//list of components contained by the game-object
	vector<Component*> components;

	//pointer to the mandatory transform component
	Transform * transform;

	/*
	* GameObject()
	* constructor, automatically creates a transform
	*/
	GameObject();

	/*
	* GameObject()
	* destructor, removes all components
	*/
	~GameObject();

	/*
	* update
	*
	* called once per frame
	* updates all components
	*
	* @param float deltaTime - the amount of passed since the last frame
	* @returns void
	*/
	void update(float deltaTime);

	/*
	* draw
	*
	* called once per frame afte the update loop
	* draws all components
	*
	* @returns void
	*/
	void draw();
};