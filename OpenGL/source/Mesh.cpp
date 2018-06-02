#include "Mesh.h"
#include "GameObject.h"
#include "Transform.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

//constructor
Mesh::Mesh(ShaderProgram * inShaderPipe)
{
	shaderPipe = inShaderPipe;
}

//destructor
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

//initialisation
void Mesh::start()
{
	initialiseQuad();
}

//set the buffers to render
void Mesh::initialiseQuad()
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
	Vertex vertices[6];

	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	//fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//define the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//unbind buffers, as a safety measure
	//this allows the vertex arrays to be bound again
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	triCount = 2;
}

//arbitary mesh initialisation
void Mesh::initialise(unsigned int vertexCount, const Vertex * vertices, unsigned int indexCount, unsigned int * indices)
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

	//fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//define the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//bind indices if there are any to bind
	if (indexCount != 0)
	{
		glGenBuffers(1, &ibo);

		//bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		//file vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		//1 triangle for every 3 indices
		triCount = indexCount / 3;
	}
	else
	{
		//triangles are defined as trios of the vertices
		triCount = vertexCount / 3;
	}

	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//main render loop
void Mesh::draw(mat4 viewProjection)
{
	shaderPipe->bind();

	//create the view matrix
	auto pvm = viewProjection * gameObject->transform->translationMatrix;
	shaderPipe->bindUniform("ProjectionViewModel", pvm);

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
