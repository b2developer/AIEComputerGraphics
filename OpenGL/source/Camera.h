#pragma once
#include "Component.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "RenderType.h"

using namespace glm;

/*
* class Camera
* child class of Component
*
* provides a virtual viewpoint for a scene to render geometry
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class Camera : public Component
{
public:

	//camera properties
	float nearPlane;
	float farPlane;
	float fieldOfView;
	float aspectRatio;
	
	//calculated properties
	mat4 projectionMatrix;
	mat4 viewMatrix;

	/*
	* Camera()
	* constructor, assigns properties
	*
	* @param float inNP - the distance of the near plane
	* @param float inFP - the distance of the far plane
	* @param float inFOV - the field-of-view of the camera
	* @param float inAR - the aspect ratio of the display the camera is rendering to
	*/
	Camera(float inNP, float inFP, float inFOV, float inAR);

	/*
	* ~Camera()
	* default destructor
	*/
	~Camera() {};

	/*
	* calculateMatrices
	*
	* reinitialises the matrices to match the member variables
	* should be called whenever a property of the camera is changed
	*
	* @returns void
	*/
	void calculateMatrices();

	/*
	* start
	* overrides Component's start()
	*
	* called when the object is created
	*
	* @returns void
	*/
	void start() override;

	/*
	* update
	* overrides Component's update(float deltaTime)
	*
	* called once per frame
	*
	* @param float deltaTime - amount of time passed since the last frame
	* @returns void
	*/
	void update(float deltaTime) override {}

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