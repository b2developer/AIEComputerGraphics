#include <iostream>
#include <stdio.h>

#include "Application.h"

using namespace glm;
using namespace aie;

int main()
{
	Application* app = new Application();
	
	//start-up the app
	if (app->startup(1280, 720, "OpenGL Application"))
	{
		//loop
		while (app->update()) {}

		//shut-down the app
		app->shutdown();
	}

	delete app;

	return 0;
}