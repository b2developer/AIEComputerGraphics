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
	SHL->postProcessingPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/post.vert").c_str());
	SHL->postProcessingPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/post.frag").c_str());

	SHL->gPassPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/standard.vert").c_str());
	SHL->gPassPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/gpass.frag").c_str());

	SHL->directionalLightPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/post.vert").c_str());
	SHL->directionalLightPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/lpass_brdf.frag").c_str());

	SHL->pointLightPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/light.vert").c_str());
	SHL->pointLightPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/lpass_brdf.frag").c_str());

	SHL->spotLightPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/light.vert").c_str());
	SHL->spotLightPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/lpass_brdf.frag").c_str());

	SHL->compositePassPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/post.vert").c_str());
	SHL->compositePassPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/composite.frag").c_str());

	SHL->tPassPipe.loadShader(aie::eShaderStage::VERTEX, std::string(rootFolder + "/shaders/standard.vert").c_str());
	SHL->tPassPipe.loadShader(aie::eShaderStage::FRAGMENT, std::string(rootFolder + "/shaders/tpass_brdf.frag").c_str());

	SHL->linkShaders();
	//------------------------------------------------------------------------------

	//colour formats for render textures
	Texture::Format formatting[11] = { Texture::Format::RGB8, Texture::Format::RGB8, Texture::Format::RGB32, Texture::Format::RGB32, //post render
									   Texture::Format::RGB32, Texture::Format::RED,Texture::Format::RGB8, Texture::Format::RED,	 //post render
									   Texture::Format::RGB32, Texture::Format::RGB32,												 //light render
									   Texture::Format::RGB32};																		 //composite render																		

	//check render target initialised properly
	if (gRender.initialise(8, width, height, formatting) == false)
	{
		return false;
	}

	if (lightRender.initialise(2, width, height, &formatting[8]) == false)
	{
		return false;
	}

	if (compRender.initialise(1, width, height, &formatting[10]) == false)
	{
		return false;
	}

	if (setupScene() == false)
	{
		return false;
	}

	INP->setCursorLockMode(ECursorLock::NONE);

	return true;
}

//generates a scene with gameobjects
bool Application::setupScene()
{

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
	GameObject* bunnyObject = new GameObject();

	bunnyObject->transform->position = vec3(10, 0, 0);
	bunnyObject->transform->onTransformUpdate();

	OBJMesh* bunnyMesh = new OBJMesh();

	//mesh failed to load
	if (bunnyMesh->load(std::string(rootFolder + "/models/Bunny.obj").c_str()) == false)
	{
		return false;
	}

	bunnyMesh->gameObject = bunnyObject;
	bunnyObject->components.push_back(bunnyMesh);
	bunnyMesh->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* dragonObject = new GameObject();

	dragonObject->transform->position = vec3(-10, 0, 0);
	dragonObject->transform->onTransformUpdate();

	OBJMesh* dragonMesh = new OBJMesh();

	//mesh failed to load
	if (dragonMesh->load(std::string(rootFolder + "/models/Dragon.obj").c_str()) == false)
	{
		return false;
	}

	dragonMesh->gameObject = dragonObject;
	dragonObject->components.push_back(dragonMesh);
	dragonMesh->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	texture = new Texture();

	//texture failed to load
	if (texture->load(std::string(rootFolder + "\\textures\\profile.png").c_str()) == false)
	{
		return false;
	}

	GameObject* quadObject = new GameObject();

	quadObject->transform->position = vec3(0, 0.0f, 0);
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

	lightRM->buffer1 = &gRender.m_targets[2];
	lightRM->buffer2 = &gRender.m_targets[3];
	lightRM->buffer3 = &gRender.m_targets[4];
	lightRM->buffer4 = &gRender.m_targets[5];

	lightRM->gameObject = lightGO;
	lightGO->components.push_back(lightRM);
	lightRM->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* compGO = new GameObject();

	RenderMesh* compRM = new RenderMesh(&compRender.m_targets[0], vec2(0.0f, 0.0f), vec2(1.0f, 1.0f), 0.0f, ERenderType::COMPOSITE_PASS);

	compRM->buffer1 = &gRender.m_targets[1];
	compRM->buffer2 = &lightRender.m_targets[0];
	compRM->buffer3 = &lightRender.m_targets[1];
	compRM->buffer4 = &gRender.m_targets[6];
	compRM->buffer5 = &gRender.m_targets[7];
	compRM->buffer6 = &gRender.m_targets[0];

	compRM->gameObject = compGO;
	compGO->components.push_back(compRM);
	compRM->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* soulspearObject = new GameObject();

	soulspearObject->transform->position = vec3(0, 0.0f, 10);
	soulspearObject->transform->onTransformUpdate();

	OBJMesh* soulspearMesh = new OBJMesh();

	soulspearMesh->useTexture = 1;
	soulspearMesh->useNormalTexture = 1;
	soulspearMesh->useSpecularTexture = 1;

	//mesh failed to load
	if (soulspearMesh->load(std::string(rootFolder + "/models/soulspear.obj").c_str(), true, true) == false)
	{
		return false;
	}

	soulspearMesh->gameObject = soulspearObject;
	soulspearObject->components.push_back(soulspearMesh);
	soulspearMesh->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* buddhaObject = new GameObject();

	buddhaObject->transform->position = vec3(-10.0f, 0.0f, 10.0f);
	buddhaObject->transform->forward = vec3(1.0f, 0.0f, 0.0f);
	buddhaObject->transform->onTransformUpdate();

	OBJMesh* buddhaMesh = new OBJMesh();

	//mesh failed to load
	if (buddhaMesh->load(std::string(rootFolder + "/models/Buddha.obj").c_str()) == false)
	{
		return false;
	}

	buddhaMesh->spin = true;
	buddhaMesh->spinSpeed = 1.0f;

	buddhaMesh->gameObject = buddhaObject;
	buddhaObject->components.push_back(buddhaMesh);
	buddhaMesh->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* lucyObject = new GameObject();

	lucyObject->transform->position = vec3(10, 0.0f, 10);
	lucyObject->transform->scale = vec3(0.5f, 0.5f, 0.5f);
	lucyObject->transform->onTransformUpdate();

	OBJMesh* lucyMesh = new OBJMesh();

	//mesh failed to load
	if (lucyMesh->load(std::string(rootFolder + "/models/Lucy.obj").c_str()) == false)
	{
		return false;
	}

	lucyMesh->gameObject = lucyObject;
	lucyObject->components.push_back(lucyMesh);
	lucyMesh->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* glassObject = new GameObject();

	glassObject->transform->position = vec3(-5, 0.0f, 7.5f);
	glassObject->transform->scale = vec3(2.0f, 2.0f, 2.0f);
	glassObject->transform->onTransformUpdate();

	OBJMesh* glassMesh = new OBJMesh();

	//mesh failed to load
	if (glassMesh->load(std::string(rootFolder + "/models/Glass.obj").c_str()) == false)
	{
		return false;
	}

	OBJMesh::Material mat;
	mat.ambient = vec3(0.01f, 0, 0);
	mat.diffuse = vec3(1, 0, 0);
	mat.opacity = 0.6f;

	glassMesh->usesAlpha = true;

	glassMesh->m_materials.clear();
	glassMesh->m_materials.push_back(mat);

	glassMesh->gameObject = glassObject;
	glassObject->components.push_back(glassMesh);
	glassMesh->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* glassObject2 = new GameObject();

	glassObject2->transform->position = vec3(10, 0.0f, 5);
	glassObject2->transform->scale = vec3(2.0f, 2.0f, 2.0f);
	glassObject2->transform->onTransformUpdate();

	OBJMesh* glassMesh2 = new OBJMesh();

	//mesh failed to load
	if (glassMesh2->load(std::string(rootFolder + "/models/Glass.obj").c_str()) == false)
	{
		return false;
	}

	mat.ambient = vec3(0, 0.01f, 0);
	mat.diffuse = vec3(0, 1, 0);
	mat.specular = vec3(1, 1, 1);
	mat.specularPower = 100.0f;
	mat.opacity = 0.3f;

	glassMesh2->usesAlpha = true;

	glassMesh2->m_materials.clear();
	glassMesh2->m_materials.push_back(mat);

	glassMesh2->gameObject = glassObject2;
	glassObject2->components.push_back(glassMesh2);
	glassMesh2->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* earthObject = new GameObject();

	earthObject->transform->position = vec3(0, 15.0f, 0);
	earthObject->transform->scale = vec3(4.0f, 4.0f, 4.0f);
	earthObject->transform->onTransformUpdate();

	OBJMesh* earthMesh = new OBJMesh();

	//mesh failed to load
	if (earthMesh->load(std::string(rootFolder + "/models/sphere.obj").c_str()) == false)
	{
		return false;
	}

	earthMesh->useTexture = 1;
	earthMesh->useAmbientTexture = 1;
	earthMesh->useNormalTexture = 1;
	earthMesh->useSpecularTexture = 1;
	earthMesh->spin = true;
	earthMesh->spinSpeed = 1.0f;

	earthMesh->m_materials[0].ambient = vec3(0.1f, 0.1f, 0.1f);
	earthMesh->m_materials[0].diffuse = vec3(1, 1, 1);
	earthMesh->m_materials[0].specular = vec3(1, 1, 1);
	earthMesh->m_materials[0].specularPower = 250.0f;

	earthMesh->m_materials[0].ambientTexture.load(std::string(rootFolder + "\\textures\\earth_lights.png").c_str());
	earthMesh->m_materials[0].diffuseTexture.load(std::string(rootFolder + "\\textures\\earth_diffuse.png").c_str());
	earthMesh->m_materials[0].normalTexture.load(std::string(rootFolder + "\\textures\\earth_normal.png").c_str());
	earthMesh->m_materials[0].specularTexture.load(std::string(rootFolder + "\\textures\\earth_specular.png").c_str());

	earthMesh->gameObject = earthObject;
	earthObject->components.push_back(earthMesh);
	earthMesh->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* earthCloudObject = new GameObject();

	earthCloudObject->transform->position = vec3(0, 15.0f, 0);
	earthCloudObject->transform->scale = vec3(4.1f, 4.1f, 4.1f);
	earthCloudObject->transform->onTransformUpdate();

	OBJMesh* earthCloudMesh = new OBJMesh();

	//mesh failed to load
	if (earthCloudMesh->load(std::string(rootFolder + "/models/sphere.obj").c_str()) == false)
	{
		return false;
	}

	earthCloudMesh->useTexture = 1;
	earthCloudMesh->spin = true;
	earthCloudMesh->spinSpeed = 0.9f;

	earthCloudMesh->m_materials[0].diffuse = vec3(1, 1, 1);
	earthCloudMesh->m_materials[0].specular = vec3(0, 0, 0);

	earthCloudMesh->m_materials[0].diffuseTexture.load(std::string(rootFolder + "\\textures\\earth_cloud.png").c_str());
	earthCloudMesh->m_materials[0].alphaTexture.load(std::string(rootFolder + "\\textures\\earth_cloud_trans.png").c_str());

	earthCloudMesh->usesAlpha = true;

	earthCloudMesh->gameObject = earthCloudObject;
	earthCloudObject->components.push_back(earthCloudMesh);
	earthCloudMesh->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* directionObject = new GameObject;

	DirectionalLight* dl = new DirectionalLight(&gRender.m_targets[2], &gRender.m_targets[3], &gRender.m_targets[4], &gRender.m_targets[5]);

	dl->deferredShader = &SHL->directionalLightPipe;
	dl->forwardShader = &SHL->tPassPipe;
	dl->direction = vec3(1, -1, 1);
	dl->diffuse = vec3(0.25f, 0.25f, 0.25f);
	dl->specular = vec3(0.25f, 0.25f, 0.25f);

	DirectionalLight* dl2 = new DirectionalLight(&gRender.m_targets[2], &gRender.m_targets[3], &gRender.m_targets[4], &gRender.m_targets[5]);

	dl2->deferredShader = &SHL->directionalLightPipe;
	dl2->forwardShader = &SHL->tPassPipe;
	dl2->direction = vec3(-1, 1, -1);
	dl2->diffuse = vec3(0.01f, 0.01f, 0.01f);
	dl2->specular = vec3(0.01f, 0.01f, 0.01f);

	dl->gameObject = directionObject;
	directionObject->components.push_back(dl);
	dl->start();

	dl2->gameObject = directionObject;
	directionObject->components.push_back(dl2);
	dl2->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* pointObject = new GameObject();

	PointLight* pl = new PointLight(&gRender.m_targets[2], &gRender.m_targets[3], &gRender.m_targets[4], &gRender.m_targets[5]);

	pl->deferredShader = &SHL->pointLightPipe;
	pl->forwardShader = &SHL->tPassPipe;
	pl->position = vec3(-3.5f, 0.5f, -8.0f);
	pl->radius = 10.0f;
	pl->diffuse = vec3(0.65f, 0, 0.65f);
	pl->specular = vec3(0.25f, 0.0f, 0.25f);

	pl->gameObject = pointObject;
	pointObject->components.push_back(pl);
	pl->start();
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GameObject* spotObject = new GameObject();

	spotObject->transform->position = vec3(10, 2.5f, -10);
	spotObject->transform->onTransformUpdate();

	SpotLight* sl = new SpotLight(&gRender.m_targets[2], &gRender.m_targets[3], &gRender.m_targets[4], &gRender.m_targets[5]);

	sl->deferredShader = &SHL->spotLightPipe;
	sl->forwardShader = &SHL->tPassPipe;
	sl->position = vec3(0, 2.5f, 0);
	sl->direction = vec3(0, 0, 1);
	sl->range = 15.0f;
	sl->minCone = 0.05f;
	sl->maxCone = 0.06f;
	sl->diffuse = vec3(0.5f, 0, 0);
	sl->specular = vec3(0.5f, 0.5f, 0.5f);

	sl->gameObject = spotObject;
	spotObject->components.push_back(sl);
	sl->start();
	//------------------------------------------------------------------------------

	scene->gameObjects.push_back(cameraObject);
	scene->gameObjects.push_back(bunnyObject);
	scene->gameObjects.push_back(dragonObject);
	scene->gameObjects.push_back(soulspearObject);
	scene->gameObjects.push_back(buddhaObject);
	scene->gameObjects.push_back(lucyObject);
	scene->gameObjects.push_back(quadObject);
	scene->gameObjects.push_back(glassObject);
	scene->gameObjects.push_back(glassObject2);
	scene->gameObjects.push_back(earthObject);
	scene->gameObjects.push_back(earthCloudObject);

	scene->gameObjects.push_back(lightGO);
	scene->gameObjects.push_back(compGO);
	scene->gameObjects.push_back(directionObject);
	scene->gameObjects.push_back(pointObject);
	scene->gameObjects.push_back(spotObject);

	scene->sortComponents();

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
void Application::clearScreen(vec4 colour, bool depthClear)
{
	if (depthClear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the screen and depth buffers
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT); //clear the screen buffer
	}

	glClearColor(colour.x, colour.y, colour.z, colour.w); //fills the screen with a solid colour
	glEnable(GL_DEPTH_TEST); //enables the depth buffer to be used this render call
}

//render function, runs after the update
void Application::draw()
{
	clearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	gRender.bind();
	clearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//clear the ambient buffer with skybox colours
	float skyboxColour[4] = { 0.01f, 0.01f, 0.01f, 1 };
	glClearTexImage(gRender.m_targets[0].getHandle(), 0, GL_RGBA, GL_FLOAT, skyboxColour);

	//render the g-pass buffers (ambient, albedo, normal, position, specular, specularPower)
	scene->draw(cameraObject, ERenderType::G_PASS);
	gRender.unbind();

	//render the t-pass buffers (combined pixel data: lighting and texturing for all transparent objects)
	gRender.bind();
	scene->draw(cameraObject, ERenderType::T_PASS);
	gRender.unbind();

	//render the l-pass (lighting for opaque objects)
	lightRender.bind();
	clearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	scene->draw(cameraObject, ERenderType::LIGHTING_PASS);
	lightRender.unbind();

	//render the composite pass (combining the g, t and l pass buffers together)
	compRender.bind();
	clearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	scene->draw(cameraObject, ERenderType::COMPOSITE_PASS);
	compRender.unbind();

	//render the post-processing pass (applies a fragment shader to a quad with the final texture for the scene)
	scene->draw(cameraObject, ERenderType::POST_PROCESSING_PASS);
}
