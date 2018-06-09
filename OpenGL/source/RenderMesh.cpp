#include "RenderMesh.h"
#include "GameObject.h"
#include "Transform.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include "ShaderLibrary.h"

//constructor
RenderMesh::RenderMesh(Texture* inTexture, vec2 inPosition, vec2 inScale, float inDepth, ERenderType inType) : position(inPosition), scale(inScale), depth(inDepth), renderingType(inType)
{
	texture = inTexture;
}

//destructor
RenderMesh::~RenderMesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

//initialisation
void RenderMesh::start()
{
	initialiseQuad();
}

//set the buffers to render
void RenderMesh::initialiseQuad()
{
	//check the mesh wasn't initialised
	assert(vao == 0);

	//reserve space for buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	//bind vertex array aka the mesh wrapper
	glBindVertexArray(vao);

	//bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//set vertices
	float vertices[12]
	{
		-1,1,
		-1,-1,
		 1,1,
		-1,-1,
		 1,-1,
		 1,1
	};

	//fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

	//define the first element as the position as a vector2
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	//unbind buffers, as a safety measure
	//this allows the vertex arrays to be bound again
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	triCount = 2;
}

//main render loop
void RenderMesh::draw(mat4 viewProjection, ERenderType renderType)
{
	if (renderType == ERenderType::POST_PROCESSING_PASS)
	{
		SHL->postProcessingPipe.bind();

		SHL->postProcessingPipe.bindUniform("position", position);
		SHL->postProcessingPipe.bindUniform("scale", scale);
		SHL->postProcessingPipe.bindUniform("depth", depth);

		SHL->postProcessingPipe.bindUniform("displayTexture", 0);

		texture->bind(0);
	}
	else if (renderType == renderingType && renderType == ERenderType::LIGHTING_PASS)
	{
		SHL->lPassPipe.bind();

		SHL->lPassPipe.bindUniform("position", vec2(0,0));
		SHL->lPassPipe.bindUniform("scale", vec2(1,1));
		SHL->lPassPipe.bindUniform("depth", 0.0f);

		SHL->lPassPipe.bindUniform("positionTexture", 1);
		SHL->lPassPipe.bindUniform("normalTexture", 2);

		texture->bind(0);

		if (buffer1 != nullptr)
		{
			buffer1->bind(1);
		}

		if (buffer2 != nullptr)
		{
			buffer2->bind(2);
		}
	}
	else if (renderType == renderingType && renderType == ERenderType::COMPOSITE_PASS)
	{

		SHL->compositePassPipe.bind();
		SHL->compositePassPipe.bindUniform("position", position);
		SHL->compositePassPipe.bindUniform("scale", scale);
		SHL->compositePassPipe.bindUniform("depth", depth);

		SHL->compositePassPipe.bindUniform("albedoTexture", 0);
		SHL->compositePassPipe.bindUniform("lightTexture", 1);

		if (buffer1 != nullptr)
		{
			buffer1->bind(0);
		}

		if (buffer2 != nullptr)
		{
			buffer2->bind(1);
		}
	}
	else
	{
		return;
	}

	glBindVertexArray(vao);

	//using indices or just vertices
	if (ibo != 0)
	{
		//draw vertices with indexing for triangle calculations
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		//draw vertices as if they are ordered as triangles
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
	}
}
