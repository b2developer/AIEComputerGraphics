#include "ShaderLibrary.h"
#include <iostream>

//get the singleton
ShaderLibrary * ShaderLibrary::getInstance()
{
	static ShaderLibrary* singleton = nullptr;

	//create the singleton if it doesn't exist yet
	if (singleton == nullptr)
	{
		singleton = new ShaderLibrary();
	}

	return singleton;
}

//error checking for initialisation
void ShaderLibrary::linkShaders()
{
	if (albedoPipe.link() == false)
	{
		std::cout << "Shader Error (Simple Shader): " << '\n' << albedoPipe.getLastError() << '\n';
	}

	if (positionPipe.link() == false)
	{
		std::cout << "Shader Error (Position Shader): " << '\n' << positionPipe.getLastError() << '\n';
	}

	if (normalPipe.link() == false)
	{
		std::cout << "Shader Error (Normal Shader): " << '\n' << normalPipe.getLastError() << '\n';
	}
}
