#pragma once
#include "Component.h"

#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace std;
using namespace glm;

/*
* class Transform
* child class of Component
* 
* identifies the translation, rotation and scale of a object
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class Transform : public Component
{
public:

	vec3 position; //translation
	vec3 forward; //rotation
	vec3 up;
	vec3 scale; //sacle

	mat4 translationMatrix;

	//array of function pointers
	vector<function<void()>> OnTransformUpdate;

	/*
	* Transform()
	* constructor, assigns properties
	*
	* @param vec3 inPosition - the position of the transform
	* @param vec3 inForward - the rotation of the transform
	* @param vec3 inUp - the up vector of the rotation
	* @param vec3 inScale - the scale of the transform
	a
	*/
	Transform(vec3 inPosition, vec3 inForward, vec3 inUp, vec3 inScale);

	/*
	* Transform()
	* default destructor
	*/
	~Transform() {}

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
	* onTransformUpdate
	*
	* should be called whenever the transform is updated
	* ensures that the matrix reflects the isolated values
	*
	* @returns void
	*/
	void onTransformUpdate();

	/*
	* update
	* override Component's update(float deltaTime)
	*
	* called once per frame
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns void
	*/
	void update(float deltaTime) override {}

	/*
	* draw
	* overrides Component's draw()
	*
	* called once per frame after the update loop
	*
	* @param Camera* camera - the pov to render the component
	* @param ERenderType renderType - the type of pass to perform on the fragment shader
	* @returns void
	*/
	void draw(Camera* camera, ERenderType renderType) override {}
};