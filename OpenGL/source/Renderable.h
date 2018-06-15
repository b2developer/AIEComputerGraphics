#pragma once
#include "Component.h"

/*
* class Renderable
* child class of Component
*
* base class of a object that can render to the screen through a scene
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class Renderable : public Component
{
public:

	/*
	* Renderable()
	* default constructor
	*/
	Renderable() {};

	/*
	* ~Renderable()
	* default destructor
	*/
	virtual ~Renderable() {}

	/*
	* start
	* virtual function
	* overrides Component's start()
	*
	* called when the object is created
	*
	* @returns void
	*/
	virtual void start() override {};

	/*
	* update
	* virtual function
	* overrides Component's update(float deltaTime)
	*
	* called once per frame
	*
	* @param float deltaTime - amount of time passed since the last frame
	* @returns void
	*/
	virtual void update(float deltaTime) override {}

	/*
	* draw
	* virtual function
	* overrides Component's draw()
	*
	* called once per frame after the update loop
	*
	* @param Camera* camera - the pov to render the component
	* @param ERenderType renderType - the type of pass to perform on the fragment shader
	* @returns void
	*/
	virtual void draw(Camera* camera, ERenderType renderType) override {};
};