#include "Application.h"

#include <iostream>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

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

	glfwMakeContextCurrent(window);

	//open gl initialisation check
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();

		return false;
	}

	//log the open gl version to the console
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();

	std::cout << "Open GL Version: " << major << "." << minor << "\n";

	//initialise gizmos
	Gizmos::create(10000, 10000, 10000, 10000);

	return true;
}

//runs when the update function stops looping
void Application::shutdown()
{
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
		//close if escape is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			return false;
		}
		
		//Game Logic
		//-------------------

		

		//-------------------

		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	else
	{
		return false;
	}

	return true;
}

#include "Camera.h"

//render function, runs after the update
void Application::draw()
{
	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(10, 0, 0), vec3(0, 1, 0));
	mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the screen and depth buffers
	glClearColor(0.25f, 0.25f, 0.25f, 1); //fills the screen with a solid colour
	glEnable(GL_DEPTH_TEST); //enables the depth buffer to be used this render call

	Camera* c = new Camera(vec3(10,10,10), vec3(0,-1,-1), 0.01f, 100.0f, glm::pi<float>() * 0.25f, 16 / 9.0f);
	c->calculateMatrices();

	Gizmos::clear(); //removes all existing gizmos

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	Gizmos::addAABBFilled(vec3(2, 2, 0), vec3(2, 2, 2), vec4(1, 0, 0, 1));

	for (int i = 0; i < 21; ++i) {

		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	Gizmos::draw(c->projectionMatrix * c->worldModelMatrix);

	delete c;
}
