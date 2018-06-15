#pragma once
#include "Renderable.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;
using namespace aie;

/*
* class RenderMesh
* child class of Renderable
*
* a mesh that ignores it's transform and always takes up the full portion
* of the screen, this can be used with render targets to create post-processing effects
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class RenderMesh : public Renderable
{
public:

	//type of render the mesh will perform
	ERenderType renderingType = ERenderType::POST_PROCESSING_PASS;

	Texture * texture;
	Texture * buffer1 = nullptr;
	Texture * buffer2 = nullptr;
	Texture * buffer3 = nullptr;
	Texture * buffer4 = nullptr;

	//amount of triangles in the mesh
	unsigned int triCount = 0;

	//handles for the gpu
	unsigned int vao = 0; //vertex array object, temporary array to copy into the vbo
	unsigned int vbo = 0; //vertex buffer objects, where vertex data is read into shaders
	unsigned int ibo = 0; //index buffer object

	//2D properties of the rendered quad
	vec2 position = vec2(0, 0);
	vec2 scale = vec2(1, 1);
	float depth = 0.0f;

	/*
	* RenderMesh()
	* constructor, assigns the shaders to bind
	*
	* @param Texture* inTexture - the texture to bind to the shader
	* @param vec2 inPosition - the centre of the quad
	* @param vec2 inScale - the span of the quad in screen space
	* @param float inDepth - the z coordinate of the render mesh, determines render order
	* @param ERenderType inType - the type of render to perform
	*/
	RenderMesh(Texture* inTexture, vec2 inPosition, vec2 inScale, float inDepth, ERenderType inType);

	/*
	* ~RenderMesh()
	* destructor, deinitialises all vertex arrays and other buffers
	*/
	~RenderMesh();

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
	* sets up the vertex buffer and normal buffers to render across the screen
	*
	* @returns void
	*/
	void initialiseQuad();

	/*
	* update
	* overrides Renderable's update(float deltaTime)
	*
	* called once per frame
	*
	* @param float deltaTime - amount of time passed since the last frame
	* @returns void
	*/
	void update(float deltaTime) override {}

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