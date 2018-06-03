#include "Application.h"

#include <iostream>
#include <stdio.h>
#include <functional>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "InputCallbacks.h"
#include "Transform.h"
#include "Camera.h"
#include "FlyCamera.h"
#include "Mesh.h"
#include "OBJMesh.h"

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
	glfwSetMouseButtonCallback(window, &onMouseButtonModified);

	char rootFolderC[FILENAME_MAX];

	ZeroMemory(rootFolderC, FILENAME_MAX);

	//get the path to the bin folder
	GetModuleFileName(NULL, rootFolderC, sizeof(rootFolderC) - 1);

	//find the first backslash from the right
	char* slash = strrchr(rootFolderC, '\\');

	//cut the string at the slash, leaving only the bin folder path and not the .exe
	*slash = '\0';

	rootFolder = rootFolderC;

	//log the open gl version to the console
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();

	std::cout << "Open GL Version: " << major << "." << minor << "\n";

	//initialise gizmos
	Gizmos::create(10000, 10000, 10000, 10000);

	shaderPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/normalmapping.vert").c_str());
	shaderPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/normalmapping.frag").c_str());

	//check the shader binded properly
	if (shaderPipe.link() == false)
	{
		cout << "Shader Error: " << '\n' << shaderPipe.getLastError() << '\n';
	}

	scene = new Scene();
	scene->shaderPipe = &shaderPipe;

	//------------------------------------------------------------------------------
	cameraObject = new GameObject();

	Camera* camera = new Camera(0.01f, 100.0f, glm::pi<float>() * 0.25f, 16 / 9.0f);
	camera->gameObject = cameraObject;
	cameraObject->components.push_back(camera);
	camera->start();

	FlyCamera* fly = new FlyCamera();
	fly->gameObject = cameraObject;
	cameraObject->components.push_back(fly);
	fly->start();

	fly->acceleration = 30.0f;
	fly->maxSpeed = 15.0f;
	fly->friction = 0.001f;
	fly->sensitivity = 6.0f;
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* meshObject = new GameObject();

	meshObject->transform->position = vec3(10, 5, 0);
	meshObject->transform->onTransformUpdate();

	OBJMesh* mesh = new OBJMesh(&shaderPipe);

	//mesh failed to load
	if (mesh->load(std::string(rootFolder + "/models/Bunny.obj").c_str()) == false)
	{
		return false;
	}

	mesh->gameObject = meshObject;
	meshObject->components.push_back(mesh);
	mesh->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* meshObject2 = new GameObject();

	meshObject2->transform->position = vec3(-10, 5, 0);
	meshObject2->transform->onTransformUpdate();

	OBJMesh* mesh2 = new OBJMesh(&shaderPipe);

	//mesh failed to load
	if (mesh2->load(std::string(rootFolder + "/models/Dragon.obj").c_str()) == false)
	{
		return false;
	}

	mesh2->gameObject = meshObject2;
	meshObject2->components.push_back(mesh2);
	mesh2->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	texture = new Texture();

	//texture failed to load
	if (texture->load(std::string(rootFolder + "\\textures\\profile.png").c_str()) == false)
	{
		return false;
	}
	
	GameObject* quadObject = new GameObject();

	quadObject->transform->position = vec3(0, 0, 0);
	quadObject->transform->scale = vec3(3, 1, 3);
	quadObject->transform->onTransformUpdate();

	Mesh* rawMesh = new Mesh(&shaderPipe, texture);
	rawMesh->gameObject = quadObject;
	quadObject->components.push_back(rawMesh);
	rawMesh->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* meshObject3 = new GameObject();

	meshObject3->transform->position = vec3(0, 5, 10);
	meshObject3->transform->onTransformUpdate();

	OBJMesh* mesh3 = new OBJMesh(&shaderPipe);

	mesh3->useTexture = 1;

	//mesh failed to load
	if (mesh3->load(std::string(rootFolder + "/models/soulspear.obj").c_str(), true, true) == false)
	{
		return false;
	}

	mesh3->gameObject = meshObject3;
	meshObject3->components.push_back(mesh3);
	mesh3->start();
	//------------------------------------------------------------------------------

	scene->gameObjects.push_back(cameraObject);
	scene->gameObjects.push_back(meshObject);
	scene->gameObjects.push_back(meshObject2);
	scene->gameObjects.push_back(meshObject3);
	//scene->gameObjects.push_back(quadObject);

	INP->setCursorLockMode(ECursorLock::NONE);

	std::cout.precision(2);
	std::cout.setf(std::ios::fixed);

	return true;
}

//runs when the update function stops looping
void Application::shutdown()
{
	delete scene;

	delete texture;

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
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			return false;
		}
		
		//Game Logic
		//-------------------
		scene->update((float)deltaTime);
		//------------------

		draw();

		INP->deltaReset();
		glfwPollEvents();
		glfwSwapBuffers(window);
	
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

	Gizmos::clear(); //removes all existing gizmos

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	scene->draw(cameraObject);

	//Gizmos::addAABBFilled(vec3(2, 2, 0), vec3(2, 2, 2), vec4(1, 0, 0, 1));

	for (int i = 0; i < 21; ++i) {

		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	Camera* c = (Camera*)cameraObject->components[1];

	mat4 viewProjectionMatrix = c->projectionMatrix * c->gameObject->transform->translationMatrix;

	Gizmos::draw(c->projectionMatrix * c->gameObject->transform->translationMatrix);
}
