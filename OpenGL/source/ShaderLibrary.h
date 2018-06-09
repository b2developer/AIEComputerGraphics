#pragma once
#include "Shader.h"

#define SHL ShaderLibrary::getInstance()

using namespace aie;

/*
* class ShaderLibrary
*
* singleton that serves as a container for shader programs
* allows for easy access to Mesh and OBJMesh objects for
* various sub-step shaders such as albedo, depth and normal texturing
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class ShaderLibrary
{
private:
	
	/*
	* ShaderLibrary()
	* default constructor
	*/
	ShaderLibrary() {}

	/*
	* ShaderLibrary()
	* default destructor
	*/
	~ShaderLibrary() {}

public: 

	ShaderProgram postProcessingPipe;
	ShaderProgram gPassPipe;
	ShaderProgram lPassPipe;
	ShaderProgram compositePassPipe;

	/*
	* getInstance
	*
	* gets the only instance of the singleton, creates it
	* if this is the first time this function is being invoked
	* 
	* @returns static Input* - the singleton instance
	*/
	static ShaderLibrary* getInstance();

	/*
	* linkShaders
	*
	* attempts to link each shader to the rendering system (OpenGL)
	* prints out any error from each attempted link
	*
	* @returns void
	*/
	void linkShaders();
};