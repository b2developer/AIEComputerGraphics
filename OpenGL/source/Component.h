#pragma once

//forward declaration
class GameObject;

/*
* class Component
*
* multiple components make up the functionality of a game-object
* each has it's own update function
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class Component
{
public:

	GameObject * gameObject;

	/*
	* Component()
	* default constructor
	*/
	Component() {};

	/*
	* ~Component()
	* default destructor
	*/
	virtual ~Component() {};

	/*
	* start
	* virtual function
	*
	* called when the object is created
	*
	* @returns void
	*/
	virtual void start() {};

	/*
	* update
	* virtual function
	*
	* called once per frame
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @param returns void
	*/
	virtual void update(float deltaTime) {};
};