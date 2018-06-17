#include "Mesh.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include "ShaderLibrary.h"

//constructor
Mesh::Mesh(Texture* inTexture)
{
	texture = inTexture;
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

	vertices[0].normal = { 0, 1.0f, 0, 1.0f};
	vertices[1].normal = { 0, 1.0f, 0, 1.0f };
	vertices[2].normal = { 0, 1.0f, 0, 1.0f };

	vertices[3].normal = { 0, 1.0f, 0, 1.0f };
	vertices[4].normal = { 0, 1.0f, 0, 1.0f };
	vertices[5].normal = { 0, 1.0f, 0, 1.0f };

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

	//define the second element as the normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4(1)) * 1));

	//define the third element as texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4(1)) * 2));

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

	//define the second element as the normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4(1)) * 1));

	//define the third element as texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4(1)) * 2));

	//bind indices if there are any to bind
	if (indexCount != 0)
	{
		glGenBuffers(1, &ibo);

		//bind index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		//file index buffer
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
void Mesh::draw(Camera* camera, ERenderType renderType)
{

	if (renderType == ERenderType::G_PASS)
	{
		SHL->gPassPipe.bind();

		SHL->gPassPipe.bindUniform("useTexture", 1);
		SHL->gPassPipe.bindUniform("useAmbientTexture", 0);
		SHL->gPassPipe.bindUniform("useNormalTexture", 0);
		SHL->gPassPipe.bindUniform("useSpecularTexture", 0);

		//create all neccessary matrice
		auto pvm = camera->viewMatrix * gameObject->transform->translationMatrix;

		SHL->gPassPipe.bindUniform("ProjectionViewModel", pvm);
		SHL->gPassPipe.bindUniform("ModelMatrix", gameObject->transform->translationMatrix);

		//create the normal matrix (rotation matrix of the model)
		mat3 nm = lookAtMatrix(vec3(0, 0, 0), gameObject->transform->forward, gameObject->transform->up);
		SHL->gPassPipe.bindUniform("NormalMatrix", nm);

		SHL->gPassPipe.bindUniform("Ka", vec3(0, 0, 0));
		SHL->gPassPipe.bindUniform("Kd", vec3(1, 1, 1));
		SHL->gPassPipe.bindUniform("Ks", vec3(0, 0, 0));
		SHL->gPassPipe.bindUniform("specularPower", 0.0f);

		SHL->gPassPipe.bindUniform("diffuseTexture", 0);

		texture->bind(0);
	}
	else if (renderType == ERenderType::T_PASS)
	{
		SHL->tPassPipe.bind();

		SHL->tPassPipe.bindUniform("useTexture", 1);
		SHL->tPassPipe.bindUniform("useAmbientTexture", 0);
		SHL->tPassPipe.bindUniform("useNormalTexture", 0);
		SHL->tPassPipe.bindUniform("useSpecularTexture", 0);
		SHL->tPassPipe.bindUniform("useAlphaTexture", 0);

		//create all neccessary matrice
		auto pvm = camera->viewMatrix * gameObject->transform->translationMatrix;

		SHL->tPassPipe.bindUniform("ProjectionViewModel", pvm);
		SHL->tPassPipe.bindUniform("ModelMatrix", gameObject->transform->translationMatrix);

		//create the normal matrix (rotation matrix of the model)
		mat3 nm = lookAtMatrix(vec3(0, 0, 0), gameObject->transform->forward, gameObject->transform->up);
		SHL->tPassPipe.bindUniform("NormalMatrix", nm);

		SHL->tPassPipe.bindUniform("Ka", vec3(0, 0, 0));
		SHL->tPassPipe.bindUniform("Kd", vec3(1, 1, 1));
		SHL->tPassPipe.bindUniform("Ks", vec3(0, 0, 0));
		SHL->tPassPipe.bindUniform("specularPower", 0.0f);

		SHL->tPassPipe.bindUniform("diffuseTexture", 0);

		texture->bind(0);
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
