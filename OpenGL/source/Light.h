#pragma once
#include "Component.h"
#include "Shader.h"

//must match the same const int in shaders
const int MAX_LIGHTS = 24;

using namespace aie;

/*
* class Light
* child class of Component
*
* base class of a object that can put light onto meshes
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class Light : public Component
{
public:

	ShaderProgram * deferredShader = nullptr;
	ShaderProgram * forwardShader = nullptr;

	/*
	* Light()
	* default constructor
	*/
	Light() {};

	/*
	* ~Light()
	* default destructor
	*/
	virtual ~Light() {}

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

	/*
	* bindLight
	* virtual function
	*
	* assigns all of it's properties to the appropriate array in shader memory
	*
	* @param int pos - the position of the array to assign to 
	* @returns void
	*/
	virtual void bindLight(int pos) {};
};