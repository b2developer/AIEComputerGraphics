#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

#include <functional>

//constructor, assigns properties
Camera::Camera(float inNP, float inFP, float inFOV, float inAR) :  nearPlane(inNP), farPlane(inFP), fieldOfView(inFOV), aspectRatio(inAR)
{
	
}

//recalculates the matrices
void Camera::calculateMatrices()
{
	projectionMatrix = glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
	viewMatrix = projectionMatrix * gameObject->transform->translationMatrix;
}

//initialisation
void Camera::start()
{
	function<void()> f = [this] { calculateMatrices(); };
	gameObject->transform->OnTransformUpdate.push_back(f);

	calculateMatrices();
}
