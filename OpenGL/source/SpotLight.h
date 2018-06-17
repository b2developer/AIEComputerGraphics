#pragma once
#include "Light.h"
#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;
using namespace aie;

/*
* class SpotLight
* child class of Light
*
* a component that takes various G-Pass buffers and creates
* colour for a light with a specific direction, cone and range
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class SpotLight : public Light
{
public:

	Texture * positionBuffer = nullptr;
	Texture * normalBuffer = nullptr;
	Texture * specularBuffer = nullptr;
	Texture * specularPowerBuffer = nullptr;

	vec3 position;
	vec3 direction;
	float range;
	float minCone;
	float maxCone;
	vec3 diffuse;
	vec3 specular;

	//amount of triangles in the mesh
	unsigned int triCount = 0;

	//handles for the gpu
	unsigned int vao = 0; //vertex array object, temporary array to copy into the vbo
	unsigned int vbo = 0; //vertex buffer objects, where vertex data is read into shaders
	unsigned int ibo = 0; //index buffer object

	/*
	* SpotLight()
	* constructor, assigns normal buffer
	*
	* @param Texture* positionBuffer - positional information
	* @param Texture* normalBuffer - normal information
	* @param Texture* specularBuffer - specular colour information
	* @param Texture* specularPowerBuffer - the specular power
	*/
	SpotLight(Texture* positionBuffer, Texture* normalBuffer, Texture* specularBuffer, Texture* specularPowerBuffer);

	/*
	* ~SpotLight()
	* destructor, deinitialises all vertex arrays and other buffers
	*/
	~SpotLight();

	/*
	* start
	* overrides Light's start()
	*
	* called when the object is created
	*
	* @returns void
	*/
	void start() override;

	/*
	* initialiseCube
	*
	* sets up the vertex buffer and normal buffers to render a cube
	* that encloses the radius the light effects
	*
	* @returns void
	*/
	void initialiseCube();

	/*
	* draw
	* overrides Light's draw()
	*
	* called once per frame after the update loop
	*
	* @param Camera* camera - the pov to render the component
	* @param ERenderType renderType - the type of pass to perform on the fragment shader
	* @returns void
	*/
	void draw(Camera* camera, ERenderType renderType) override;

	/*
	* bindLight
	* overrides Light's bindLight(int position)
	*
	* assigns all of it's properties to the appropriate array in shader memory
	*
	* @param int position - the position of the array to assign to
	* @returns void
	*/
	void bindLight(int position) override;
};