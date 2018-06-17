#pragma once

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Gizmos.h"

#include "Shader.h"
#include "RenderTarget.h"

#include "Scene.h"
#include "GameObject.h"

#include "Texture.h"

using namespace glm;
using namespace aie;

const int MAX_NAME = 200;

/*
* class Application
*
* initialises all necessary libraries for rendering, display and input
* then runs constant updates that refresh the screen
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class Application
{
private:

	double lastTime = 0.0f;

public:

	//window of the application
	GLFWwindow * window;

	std::string rootFolder;

	Scene* scene;
	GameObject* cameraObject;

	Texture* texture;
	Texture* texture2;

	RenderTarget gRender;
	RenderTarget lightRender;
	RenderTarget compRender;
	RenderTarget transRender;

	/*
	* Application()
	* default constructor
	*/
	Application();

	/*
	* ~Application()
	* default destructor
	*/
	~Application();

	/*
	* startup
	*
	* attempts to initialise all necessary library functions
	* such as the GLFW window and the OpenGL rendering
	*
	* @param unsigned int width - the width in pixels
	* @param unsigned int height - the height in pixels
	* @param const char windowName[MAX_NAME] - the display name of the window
	* @returns bool - startup attempt result, true means the application can load
	*/
	bool startup(unsigned int width, unsigned int height, const char windowName[MAX_NAME]);

	/*
	* setupScene
	*
	* creates a scene with several game-objects that demonstrate the rendering engine
	*
	* @returns bool - indicator of the scene seting up correctly
	*/
	bool setupScene();

	/*
	* shutdown
	*
	* runs the application stops updating, should be used for deinitialisation of
	* any objects that have been assigned to the heap (textures, managers, static objects)
	*
	* @returns void
	*/
	void shutdown();

	/*
	* update
	*
	* the main loop of the application, if it is successful draw should be called
	* otherwise the application should be stopped
	*
	* @returns bool - result of the update, false indicates shutdown
	*/
	bool update();

	/*
	* clearScreen
	*
	* tells OpenGL to clear the colour and depth buffers
	* and fills the screen with a solid colour
	*
	* @param vec4 colour - the colour to fill the screen with
	* @param bool depthClear - flag indicating whether the depth buffer should be cleared
	* @returns void
	*/
	void clearScreen(vec4 colour, bool depthClear = true);

	/*
	* draw
	*
	* rendering loop of the application, draws all objects to the screen
	*
	* @returns void
	*/
	void draw();
};