#include "Transform.h"

//constructor
Transform::Transform(vec3 inPosition, vec3 inForward, vec3 inUp, vec3 inScale) : position(inPosition), forward(inForward), up(inUp), scale(inScale)
{
	translationMatrix = lookAtMatrix(position, forward, up, inverted);

	mat4 scaleMatrix = mat4(1);

	//scale using the scale vector
	scaleMatrix[0][0] = scale.x;
	scaleMatrix[1][1] = scale.y;
	scaleMatrix[2][2] = scale.z;

	translationMatrix = translationMatrix * scaleMatrix;

}

//called whenever an update is called
void Transform::onTransformUpdate()
{
	translationMatrix = lookAtMatrix(position, forward, up, inverted);

	mat4 scaleMatrix = mat4(1);

	//scale using the scale vector
	scaleMatrix[0][0] = scale.x;
	scaleMatrix[1][1] = scale.y;
	scaleMatrix[2][2] = scale.z;

	translationMatrix = translationMatrix * scaleMatrix;

	//iterate through the functions, calling each
	for (vector<function<void()>>::iterator iter = OnTransformUpdate.begin(); iter != OnTransformUpdate.end(); iter++)
	{
		(*iter)();
	}
}

//look at matrix generation
mat4 lookAtMatrix(vec3 inPosition, vec3 inForward, vec3 inUp, bool inverted)
{
	mat4 matrix(1.0f);

	//generate the translation matrix
	vec3 f = normalize(inForward);

	//rotation using forward and up vectors
	vec3 nf = -inForward;

	vec3 side = normalize(cross(f, inUp));

	matrix[0][0] = side.x;
	matrix[1][0] = side.y;
	matrix[2][0] = side.z;

	vec3 upC = cross(side, f);

	matrix[0][1] = upC.x;
	matrix[1][1] = upC.y;
	matrix[2][1] = upC.z;

	matrix[0][2] = nf.x;
	matrix[1][2] = nf.y;
	matrix[2][2] = nf.z;

	if (inverted)
	{
		matrix[3][0] = -dot(side, inPosition);
		matrix[3][1] = -dot(upC, inPosition);
		matrix[3][2] = dot(f, inPosition);
	}
	else
	{
		matrix[3][0] = inPosition.x;
		matrix[3][1] = inPosition.y;
		matrix[3][2] = inPosition.z;
	}

	return matrix;
}