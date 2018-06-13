#include "Application.h"

#include <iostream>
#include <stdio.h>
#include <functional>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "InputCallbacks.h"
#include "ShaderLibrary.h"
#include "Transform.h"
#include "Camera.h"
#include "FlyCamera.h"

#include "Mesh.h"
#include "OBJMesh.h"
#include "RenderMesh.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

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
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed);

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

	//load essential shaders for lighting
	//------------------------------------------------------------------------------
	SHL->postProcessingPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/advanced/post.vert").c_str());
	SHL->postProcessingPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/advanced/post.frag").c_str());

	SHL->gPassPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/advanced/standard.vert").c_str());
	SHL->gPassPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/advanced/gpass.frag").c_str());

	SHL->directionalLightPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/advanced/post.vert").c_str());
	SHL->directionalLightPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/advanced/brdf.frag").c_str());

	SHL->pointLightPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/advanced/light.vert").c_str());
	SHL->pointLightPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/advanced/brdf_point.frag").c_str());

	SHL->spotLightPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/advanced/light.vert").c_str());
	SHL->spotLightPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/advanced/brdf_spot.frag").c_str());

	SHL->compositePassPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/advanced/post.vert").c_str());
	SHL->compositePassPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/advanced/composite.frag").c_str());

	SHL->linkShaders();
	//------------------------------------------------------------------------------

	//colour formats for render textures
	Texture::Format formatting[8] = { Texture::Format::RGB8, Texture::Format::RGB32, Texture::Format::RGB32, Texture::Format::RGB32, Texture::Format::RED,	//post render
									  Texture::Format::RGBA, Texture::Format::RGB32,																		//light render
									  Texture::Format::RGBA};																								//composite render


	//check render target initialised properly
	if (postRender.initialise(5, width, height, formatting) == false)
	{
		return false;
	}

	if (lightRender.initialise(2, width, height, &formatting[5]) == false)
	{
		return false;
	}

	if (compRender.initialise(1, width, height, &formatting[7]) == false)
	{
		return false;
	}

	scene = new Scene();

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

	OBJMesh* mesh = new OBJMesh();

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

	OBJMesh* mesh2 = new OBJMesh();

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

	quadObject->transform->position = vec3(0, -1, 0);
	quadObject->transform->scale = vec3(3, 1, 3);
	quadObject->transform->onTransformUpdate();

	Mesh* rawMesh = new Mesh(texture);
	rawMesh->gameObject = quadObject;
	quadObject->components.push_back(rawMesh);
	rawMesh->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* lightGO = new GameObject();

	RenderMesh* lightRM = new RenderMesh(nullptr, vec2(1 - 0.333f, 1 - 0.666f), vec2(0.333f, 0.333f), 0.0f, ERenderType::LIGHTING_PASS);

	lightRM->buffer1 = &postRender.m_targets[1];
	lightRM->buffer2 = &postRender.m_targets[2];
	lightRM->buffer3 = &postRender.m_targets[3];
	lightRM->buffer4 = &postRender.m_targets[4];

	lightRM->gameObject = lightGO;
	lightGO->components.push_back(lightRM);
	lightRM->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* compGO = new GameObject();

	RenderMesh* compRM = new RenderMesh(&compRender.m_targets[0], vec2(0.0f, 0.0f), vec2(1.0f, 1.0f), 0.0f, ERenderType::COMPOSITE_PASS);

	compRM->buffer1 = &postRender.m_targets[0];
	compRM->buffer2 = &lightRender.m_targets[0];
	compRM->buffer3 = &lightRender.m_targets[1];

	compRM->gameObject = compGO;
	compGO->components.push_back(compRM);
	compRM->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* meshObject3 = new GameObject();

	meshObject3->transform->position = vec3(0, 5, 10);
	meshObject3->transform->onTransformUpdate();

	OBJMesh* mesh3 = new OBJMesh();

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

	//------------------------------------------------------------------------------
	GameObject* directionObject = new GameObject;

	DirectionalLight* dl = new DirectionalLight(&SHL->directionalLightPipe, &postRender.m_targets[1], &postRender.m_targets[2], &postRender.m_targets[3], &postRender.m_targets[4]);

	dl->direction = vec3(1, -1, 1);
	dl->diffuse = vec3(0.25f, 0.25f, 0.25f);
	dl->specular = vec3(0.25f, 0.25f, 0.25f);

	dl->gameObject = directionObject;
	directionObject->components.push_back(dl);
	dl->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* pointObject = new GameObject();

	PointLight* pl = new PointLight(&SHL->pointLightPipe, &postRender.m_targets[1], &postRender.m_targets[2], &postRender.m_targets[3], &postRender.m_targets[4]);

	pl->position = vec3(-3.5, 0, 0);
	pl->radius = 10.0f;
	pl->diffuse = vec3(0.25f, 0, 0.25f);
	pl->specular = vec3(0.25f, 0.25f, 0.25f);

	pl->gameObject = pointObject;
	pointObject->components.push_back(pl);
	pl->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* spotObject = new GameObject();

	spotObject->transform->position = vec3(10, 0, -5);
	spotObject->transform->forward = glm::normalize(vec3(0.1f, 0.0f, 0.9f));
	spotObject->transform->onTransformUpdate();

	SpotLight* sl = new SpotLight(&SHL->spotLightPipe, &postRender.m_targets[1], &postRender.m_targets[2], &postRender.m_targets[3], &postRender.m_targets[4]);
	
	sl->position = vec3(0, 0, 0);
	sl->direction = vec3(0, 0, 1);
	sl->range = 15.0f;
	sl->minCone = 0.1f;
	sl->maxCone = 0.11f;
	sl->diffuse = vec3(0.5f, 0, 0);
	sl->specular = vec3(0.5f, 0.5f, 0.5f);

	sl->gameObject = spotObject;
	spotObject->components.push_back(sl);
	sl->start();
	//------------------------------------------------------------------------------

	scene->gameObjects.push_back(cameraObject);
	scene->gameObjects.push_back(meshObject);
	scene->gameObjects.push_back(meshObject2);
	scene->gameObjects.push_back(meshObject3);
	scene->gameObjects.push_back(quadObject);

	scene->gameObjects.push_back(lightGO);
	scene->gameObjects.push_back(compGO);
	scene->gameObjects.push_back(directionObject);
	scene->gameObjects.push_back(pointObject);
	scene->gameObjects.push_back(spotObject);

	INP->setCursorLockMode(ECursorLock::NONE);

	return true;
}

//runs when the update function stops looping
void Application::shutdown()
{
	delete scene;

	delete texture;
	delete texture2;

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

//clears the screen and fills it with a solid colour
void Application::clearScreen(vec4 colour)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the screen and depth buffers
	glClearColor(colour.x, colour.y, colour.z, colour.w); //fills the screen with a solid colour
	glEnable(GL_DEPTH_TEST); //enables the depth buffer to be used this render call
}

//render function, runs after the update
void Application::draw()
{

	clearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	postRender.bind();
	clearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	scene->draw(cameraObject, ERenderType::G_PASS);
	postRender.unbind();

	lightRender.bind();
	clearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	scene->draw(cameraObject, ERenderType::LIGHTING_PASS);
	lightRender.unbind();

	compRender.bind();
	clearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	scene->draw(cameraObject, ERenderType::COMPOSITE_PASS);
	compRender.unbind();

	scene->draw(cameraObject, ERenderType::POST_PROCESSING_PASS);

	/*
	Gizmos::clear(); //removes all existing gizmos

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i) 
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	Camera* c = (Camera*)cameraObject->components[1];

	mat4 viewProjectionMatrix = c->projectionMatrix * c->gameObject->transform->translationMatrix;

	Gizmos::draw(c->projectionMatrix * c->gameObject->transform->translationMatrix);
	*/
}
