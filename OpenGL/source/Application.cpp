#include "Application.h"

#include <iostream>
#include <stdio.h>
#include <functional>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "InputCallbacks.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

//constructor
Application::Application()
{

}

//destructor
Application::~Application()
{

}

//runs when the update function will start looping
bool Application::startup(unsigned int width, unsigned int height, const char windowName[MAX_NAME])
{
	//window system initialisation
	if (glfwInit() == false)
	{
		return false;
	}

	//create the window
	window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);

	//window initialisation check
	if (window == nullptr)
	{
		return false;
	}

	INP->gameWindow = window;

	glfwMakeContextCurrent(window);

	//open gl initialisation check
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();

		return false;
	}

	glfwSetKeyCallback(window, &onKeyModified);
	glfwSetCursorPosCallback(window, &onMouseModified);

	//log the open gl version to the console
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();

	std::cout << "Open GL Version: " << major << "." << minor << "\n";

	//initialise gizmos
	Gizmos::create(10000, 10000, 10000, 10000);

	scene = new Scene();

	cameraObject = new GameObject();

	Camera* camera = new Camera(0.01f, 100.0f, glm::pi<float>() * 0.25f, 16 / 9.0f);
	camera->gameObject = cameraObject;
	cameraObject->components.push_back(camera);
	camera->start();

	INP->setCursorLockMode(ECursorLock::NONE);

	return true;
}

//runs when the update function stops looping
void Application::shutdown()
{
	delete scene;

	//destroy the window
	glfwDestroyWindow(window);
	glfwTerminate();
}

//main update loop
bool Application::update()
{
	//window isn't trying to close itself
	if (glfwWindowShouldClose(window) == false)
	{
		double time = glfwGetTime();

		//calculate the different in the time points to get dt
		double deltaTime = time - lastTime;

		//close if escape is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			return false;
		}
		
		//Game Logic
		//-------------------
		scene->update((float)deltaTime);
		//-------------------

		draw();

		INP->deltaReset();

		glfwSwapBuffers(window);
		glfwPollEvents();

		lastTime = time;
	}
	else
	{
		return false;
	}

	return true;
}

//render function, runs after the update
void Application::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the screen and depth buffers
	glClearColor(0.25f, 0.25f, 0.25f, 1); //fills the screen with a solid colour
	glEnable(GL_DEPTH_TEST); //enables the depth buffer to be used this render call

	scene->draw();

	Camera* c = (Camera*)cameraObject->components[1];

	Gizmos::clear(); //removes all existing gizmos

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	Gizmos::addAABBFilled(vec3(2, 2, 0), vec3(2, 2, 2), vec4(1, 0, 0, 1));

	for (int i = 0; i < 21; ++i) {

		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	Gizmos::draw(c->projectionMatrix * c->gameObject->transform->translationMatrix);
}
