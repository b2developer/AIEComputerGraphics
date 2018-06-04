#pragma once
#include "Component.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;

/*
* class FlyCamera
* child class of Component
*
* controls the motion of a game-object using keyboard input
* and the rotation of the game-object using mouse input
*
* @author: Bradley Booth, Academy of Interactive Entertaint, 2018
*/
class FlyCamera : public Component
{
public:

	//linear statistics
	vec3 velocity;
	float acceleration;
	float maxSpeed;
	float friction;

	//rotation statistics
	vec2 rotationAngles;
	float sensitivity;

	//key mappings
	int forwardCode = GLFW_KEY_W;
	int backwardCode = GLFW_KEY_S;
	int leftCode = GLFW_KEY_A;
	int rightCode = GLFW_KEY_D;
	int upCode = GLFW_KEY_SPACE;
	int downCode = GLFW_KEY_LEFT_SHIFT;

	//key mappings concerning mouse attachment
	int escapeCode = GLFW_KEY_ESCAPE;
	int attackCode = GLFW_MOUSE_BUTTON_LEFT;

	/*
	* FlyCamera()
	* default constructor
	*/
	FlyCamera() {}

	/*
	* ~FlyCamera()
	* default destructor
	*/
	~FlyCamera() {}

	/*
	* start
	* overrides Component's start()
	*
	* called when the object is created
	*
	* @returns void
	*/
	void start() override {}
	
	/*
	* update
	* overrides Component's update(float deltaTime)
	*
	* called once per frame
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @param returns void
	*/
	void update(float deltaTime) override;

	/*
	* draw
	* overrides Component's draw()
	*
	* called once per frame after the update loop
	*
	* @param mat4 viewProjection - the camera matrix
	* @param ERenderType renderType - the type of pass to perform on the fragment shader
	* @returns void
	*/
	void draw(mat4 viewProjection, ERenderType renderType) override {}
};