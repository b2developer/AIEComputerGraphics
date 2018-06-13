#include "Transform.h"

//constructor
Transform::Transform(vec3 inPosition, vec3 inForward, vec3 inUp, vec3 inScale) : position(inPosition), forward(inForward), up(inUp), scale(inScale)
{
	//generate the translation matrix
	vec3 f = normalize(forward);

	//rotation using forward and up vectors
	vec3 nf = -forward;

	vec3 side = normalize(cross(f, up));

	translationMatrix[0][0] = side.x;
	translationMatrix[1][0] = side.y;
	translationMatrix[2][0] = side.z;

	vec3 upC = cross(side, f);

	translationMatrix[0][1] = upC.x;
	translationMatrix[1][1] = upC.y;
	translationMatrix[2][1] = upC.z;

	translationMatrix[0][2] = nf.x;
	translationMatrix[1][2] = nf.y;
	translationMatrix[2][2] = nf.z;

	translationMatrix[3][0] = -dot(side, position);
	translationMatrix[3][1] = -dot(upC, position);
	translationMatrix[3][2] = dot(f, position);
	translationMatrix[3][3] = 1.0f;

	//scale using the scale vector
	translationMatrix[0][0] *= scale.x;
	translationMatrix[1][1] *= scale.y;
	translationMatrix[2][2] *= scale.z;

}

//called whenever an update is called
void Transform::onTransformUpdate()
{
	//generate the translation matrix
	vec3 f = normalize(forward);

	//rotation using forward and up vectors
	vec3 nf = -forward;

	vec3 side = normalize(cross(f, up));

	translationMatrix[0][0] = side.x;
	translationMatrix[1][0] = side.y;
	translationMatrix[2][0] = side.z;

	vec3 upC = cross(side, f);

	translationMatrix[0][1] = upC.x;
	translationMatrix[1][1] = upC.y;
	translationMatrix[2][1] = upC.z;

	translationMatrix[0][2] = nf.x;
	translationMatrix[1][2] = nf.y;
	translationMatrix[2][2] = nf.z;

	translationMatrix[3][0] = -dot(side, position);
	translationMatrix[3][1] = -dot(upC, position);
	translationMatrix[3][2] = dot(f, position);
	translationMatrix[3][3] = 1.0f;

	//scale using the scale vector
	translationMatrix[0][0] *= scale.x;
	translationMatrix[1][1] *= scale.y;
	translationMatrix[2][2] *= scale.z;

	//iterate through the functions, calling each
	for (vector<function<void()>>::iterator iter = OnTransformUpdate.begin(); iter != OnTransformUpdate.end(); iter++)
	{
		(*iter)();
	}
}
