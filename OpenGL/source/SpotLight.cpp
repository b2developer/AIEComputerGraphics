#include "SpotLight.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <string>

#include "ShaderLibrary.h"

//constructor
SpotLight::SpotLight(Texture* inPosition, Texture* inNormal, Texture* inSpecular, Texture* inSpecularP) : positionBuffer(inPosition),
normalBuffer(inNormal), specularBuffer(inSpecular), specularPowerBuffer(inSpecularP)
{

}

//destructor
SpotLight::~SpotLight()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

//initialisation
void SpotLight::start()
{
	initialiseCube();
}

//set the buffers to render
void SpotLight::initialiseCube()
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

	//set vertices for a cube
	float cubeVertices[] = { -1,  -1,  1,  1,  1,  -1,  1,  1,  1,  -1,  -1,  1,      -1,  -1,  -1,  1,      -1,  1,  1,  1,      1,  1,  1,  1,  1,  1,  -1,  1,    -1,  1,  -1,  1, };

	unsigned int cubeIndices[] = { 0,  5,  4,
		0,  1,  5,
		1,  6,  5,
		1,  2,  6,
		2,  7,  6,
		2,  3,  7,
		3,  4,  7,
		3,  0,  4,
		4,  6,  7,
		4,  5,  6,
		3,  1,  0,
		3,  2,  1 };

	//fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), cubeVertices, GL_STATIC_DRAW);

	//define the first element as the position as a vector4
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), cubeIndices, GL_STATIC_DRAW);

	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	triCount = 12;
}

//main render loop
void SpotLight::draw(Camera* camera, ERenderType renderType)
{
	if (renderType == ERenderType::LIGHTING_PASS)
	{
		glCullFace(GL_FRONT);

		deferredShader->bind();

		deferredShader->bindUniform("lightType", 2);

		deferredShader->bindUniform("cameraPosition", camera->gameObject->transform->position);

		auto pvm = camera->viewMatrix * gameObject->transform->translationMatrix;
		deferredShader->bindUniform("ProjectionViewModel", pvm);

		//create the normal matrix (rotation matrix of the model)
		mat3 nm = lookAtMatrix(vec3(0, 0, 0), gameObject->transform->forward, gameObject->transform->up);

		//rotatated light position
		vec3 nlp = position * nm;
		vec3 nld = direction * nm;

		//simple trigonometry to find the horizonal span
		float span = range / cosf(maxCone);

		deferredShader->bindUniform("scale", vec3(span, span, range) * 2.0f);
		deferredShader->bindUniform("lightPosition", gameObject->transform->position + nlp);
		deferredShader->bindUniform("lightDirection", nld);
		deferredShader->bindUniform("lightDiffuse", diffuse);
		deferredShader->bindUniform("lightSpecular", specular);

		deferredShader->bindUniform("range", range);
		deferredShader->bindUniform("minCone", minCone);
		deferredShader->bindUniform("maxCone", maxCone);

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

	glCullFace(GL_BACK);
}

//assigns in-shader properties to an array for this light
void SpotLight::bindLight(int pos)
{
	string header = "lights[" + to_string(pos) + "].";

	string targetString = header + "lightType";
	forwardShader->bindUniform(targetString.c_str(), 2);

	//create the normal matrix (rotation matrix of the model)
	mat3 nm = lookAtMatrix(vec3(0, 0, 0), gameObject->transform->forward, gameObject->transform->up);

	//rotatated light position
	vec3 nlp = position * nm;
	vec3 nld = direction * nm;

	targetString = header + "lightPosition";
	forwardShader->bindUniform(targetString.c_str(), nlp);

	targetString = header + "lightDirection";
	forwardShader->bindUniform(targetString.c_str(), nld);

	targetString = header + "lightDiffuse";
	forwardShader->bindUniform(targetString.c_str(), diffuse);

	targetString = header + "lightSpecular";
	forwardShader->bindUniform(targetString.c_str(), specular);

	targetString = header + "range";
	forwardShader->bindUniform(targetString.c_str(), range);

	targetString = header + "minCone";
	forwardShader->bindUniform(targetString.c_str(), minCone);

	targetString = header + "maxCone";
	forwardShader->bindUniform(targetString.c_str(), maxCone);
}
