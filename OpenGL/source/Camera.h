#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;

/*
* class Camera
*
* provides a virtual viewpoint for a scene to render geometry
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*
a
*/
class Camera
{
public:

	//entity properties
	vec3 position;
	vec3 forward;

	//camera properties
	float nearPlane;
	float farPlane;
	float fieldOfView;
	float aspectRatio;
	
	//calculated properties
	mat4 projectionMatrix;
	mat4 worldModelMatrix;

	/*
	* Camera()
	* constructor, assigns properties
	*
	* @param vec3 inPosition - the position of the camera
	* @param vec3 inForward - the forward direction of the camera
	* @param float inNP - the distance of the near plane
	* @param float inFP - the distance of the far plane
	* @param float inFOV - the field-of-view of the camera
	* @param float inAR - the aspect ratio of the display the camera is rendering to
	*/
	Camera(vec3 inPosition, vec3 inForward, float inNP, float inFP, float inFOV, float inAR);

	/*
	* ~Camera()
	* default destructor
	*/
	~Camera();

	/*
	* calculateMatrices
	*
	* reinitialises the matrices to match the member variables
	* should be called whenever a property of the camera is changed
	*
	* @returns void
	*/
	void calculateMatrices();

};