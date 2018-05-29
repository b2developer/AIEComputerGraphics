#include "Camera.h"

//constructor, assigns properties
Camera::Camera(vec3 inPosition, vec3 inForward, float inNP, float inFP, float inFOV, float inAR) : position(inPosition), forward(inForward), nearPlane(inNP), farPlane(inFP),
																								   fieldOfView(inFOV), aspectRatio(inAR)
{

}

//destructor
Camera::~Camera()
{

}

//recalculates the matrices
void Camera::calculateMatrices()
{
	worldModelMatrix = glm::lookAt(position, position + forward, vec3(0, 1, 0));
	projectionMatrix = glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
}
