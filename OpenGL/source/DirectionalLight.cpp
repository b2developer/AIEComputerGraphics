#include "DirectionalLight.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include "ShaderLibrary.h"

//constructor
DirectionalLight::DirectionalLight(Texture* inPosition, Texture* inNormal, Texture* inSpecular, Texture* inSpecularP) : positionBuffer(inPosition),
																														normalBuffer(inNormal), specularBuffer(inSpecular), specularPowerBuffer(inSpecularP)
{
	
}

//destructor
DirectionalLight::~DirectionalLight()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

//initialisation
void DirectionalLight::start()
{
	initialiseQuad();
}

//set the buffers to render
void DirectionalLight::initialiseQuad()
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
void DirectionalLight::draw(Camera* camera, ERenderType renderType)
{
	if (renderType == ERenderType::LIGHTING_PASS)
	{		
		deferredShader->bind();

		deferredShader->bindUniform("lightType", 0);

		deferredShader->bindUniform("position", vec2(0, 0));
		deferredShader->bindUniform("scale", vec2(1, 1));
		deferredShader->bindUniform("depth", 0.0f);

		deferredShader->bindUniform("cameraPosition", camera->gameObject->transform->position);

		deferredShader->bindUniform("lightDirection", direction);
		deferredShader->bindUniform("lightDiffuse", diffuse);
		deferredShader->bindUniform("lightSpecular", specular);

		deferredShader->bindUniform("positionTexture", 0);
		deferredShader->bindUniform("normalTexture", 1);
		deferredShader->bindUniform("specularTexture", 2);
		deferredShader->bindUniform("specularPowerTexture", 3);

		positionBuffer->bind(0);
		normalBuffer->bind(1);
		specularBuffer->bind(2);
		specularPowerBuffer->bind(3);
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

//assigns in-shader properties to an array for this light
void DirectionalLight::bindLight(int pos)
{
	string header = "lights[" + to_string(pos) + "].";

	string targetString = header + "lightType";
	forwardShader->bindUniform(targetString.c_str(), 0);

	targetString = header + "lightDirection";
	forwardShader->bindUniform(targetString.c_str(), direction);

	targetString = header + "lightDiffuse";
	forwardShader->bindUniform(targetString.c_str(), diffuse);

	targetString = header + "lightSpecular";
	forwardShader->bindUniform(targetString.c_str(), specular);

}