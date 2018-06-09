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
	if (postProcessingPipe.link() == false)
	{
		std::cout << "Shader Error (Post-Processing Shader): " << '\n' << postProcessingPipe.getLastError() << '\n';
	}

	if (gPassPipe.link() == false)
	{
		std::cout << "Shader Error (G-Pass Shader): " << '\n' << gPassPipe.getLastError() << '\n';
	}

	if (lPassPipe.link() == false)
	{
		std::cout << "Shader Error (L-Pass Shader): " << '\n' << lPassPipe.getLastError() << '\n';
	}

	if (compositePassPipe.link() == false)
	{
		std::cout << "Shader Error (Composite Shader): " << '\n' << compositePassPipe.getLastError() << '\n';
	}
}
