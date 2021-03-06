#pragma once
#include "Renderable.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;
using namespace aie;

/*
* class Mesh
* child class of Renderable
*
* a collection of vertices that create the surfaces of a 3D model
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class Mesh : public Renderable
{
public:

	Texture * texture;

	//amount of triangles in the mesh
	unsigned int triCount = 0;

	//handles for the gpu
	unsigned int vao = 0; //vertex array object, temporary array to copy into the vbo
	unsigned int vbo = 0; //vertex buffer objects, where vertex data is read into shaders
	unsigned int ibo = 0; //index buffer object

	//sub-structure of mesh, passed into shader code
	struct Vertex
	{
		vec4 position;
		vec4 normal;
		vec2 texCoord;
	};

	/*
	* Mesh()
	* constructor, assigns the shaders to bind
	*
	* @param Texture* inTexture - the texture to bind to the shader
	*/
	Mesh(Texture* inTexture);

	/*
	* ~Mesh()
	* destructor, deinitialises all vertex arrays and other buffers
	*/
	~Mesh();

	/*
	* start
	* overrides Renderable's start()
	*
	* called when the object is created
	*
	* @returns void
	*/
	void start() override;

	/*
	* initialiseQuad
	*
	* sets up the vertex buffer and normal buffers to render two triangles
	*
	* @returns void
	*/
	void initialiseQuad();

	/*
	* initialise
	*
	* arbitary mesh initialisation
	* generates vertex and index buffers given the vertex list and index list of the mesh
	*
	* @param unsigned int vertexCount - the amount of vertices
	* @param const Vertex* vertices - the list of vertices
	* @param unsigned int indexCount- the amount of indices
	* @param unsigned int* - pointer to the array of indices
	* @returns void
	*/
	void initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);

	/*
	* draw
	* overrides Renderable's draw()
	*
	* called once per frame after the update loop
	*
	* @param Camera* camera - the pov to render the component
	* @param ERenderType renderType - the type of pass to perform on the fragment shader
	* @returns void
	*/
	void draw(Camera* camera, ERenderType renderType) override;
};