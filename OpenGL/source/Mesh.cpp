#include "Mesh.h"
#include "GameObject.h"
#include "Transform.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include "ShaderLibrary.h"

//constructor
Mesh::Mesh(Texture* inTexture)
{

	kD = new float[4];
	kD[0] = 1.0f;
	kD[1] = 0.0f;
	kD[2] = 1.0f;
	kD[3] = 1.0f;

	texture = inTexture;
}

//destructor
Mesh::~Mesh()
{
	delete kD;

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

	vertices[0].texCoord = { 0, 1 };
	vertices[1].texCoord = { 1, 1 };
	vertices[2].texCoord = { 0, 0 };

	vertices[3].texCoord = { 0, 0 };
	vertices[4].texCoord = { 1, 1 };
	vertices[5].texCoord = { 1, 0 };

	//fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//define the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//define the third element as texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

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

	//define the third element as texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

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
void Mesh::draw(mat4 viewProjection, ERenderType renderType)
{
	//albedo pass
	if (renderType == ERenderType::ALBEDO_PASS)
	{
		SHL->albedoPipe.bind();

		SHL->albedoPipe.bindUniform("useTexture", 1);

		//create the view matrix
		auto pvm = viewProjection * gameObject->transform->translationMatrix;
		SHL->albedoPipe.bindUniform("ProjectionViewModel", pvm);

		SHL->albedoPipe.bindUniform("Kd", vec3(1, 1, 1));
		SHL->albedoPipe.bindUniform("diffuseTexture", 0);

		texture->bind(0);
	}
	else if (renderType == ERenderType::POSITION_PASS)
	{
		SHL->positionPipe.bind();

		//create the view matrix
		auto pvm = viewProjection * gameObject->transform->translationMatrix;
		SHL->positionPipe.bindUniform("ProjectionViewModel", pvm);
		SHL->positionPipe.bindUniform("ModelMatrix", gameObject->transform->translationMatrix);

	}
	else if (renderType == ERenderType::NORMAL_PASS)
	{
		SHL->normalPipe.bind();

		SHL->normalPipe.bindUniform("useTexture", 0);

		//create the view matrix
		auto pvm = viewProjection * gameObject->transform->translationMatrix;
		SHL->normalPipe.bindUniform("ProjectionViewModel", pvm);

		//create the normal matrix (rotation matrix of the model)
		mat3 nm = glm::lookAt(vec3(0, 0, 0), gameObject->transform->forward, vec3(0, 1, 0));
		SHL->normalPipe.bindUniform("NormalMatrix", nm);
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
