#include "Transform.h"

//constructor
Transform::Transform(vec3 inPosition, vec3 inForward, vec3 inUp, vec3 inScale) : position(inPosition), forward(inForward), up(inUp), scale(inScale)
{

}

//called whenever an update is called
void Transform::onTransformUpdate()
{
	//generate the translation matrix
	translationMatrix = glm::lookAt(position, forward, up);
	translationMatrix[0] *= scale.x;
	translationMatrix[5] *= scale.y;
	translationMatrix[10] *= scale.z;

	//iterate through the functions, calling each
	for (vector<function<void()>>::iterator iter = OnTransformUpdate.begin(); iter != OnTransformUpdate.end(); iter++)
	{
		(*iter)();
	}
}
