#include "Input.h"

//get the singleton
Input * Input::getInstance()
{
	static Input* singleton = nullptr;

	//create the singleton if it doesn't exist yet
	if (singleton == nullptr)
	{
		singleton = new Input();
	}

	return singleton;
}

//delta reset
void Input::deltaReset()
{
	mouseMovementX = 0.0;
	mouseMovementY = 0.0;
}

//set lock mode of the cursor
void Input::setCursorLockMode(ECursorLock mode)
{
	cursorMode = mode;

	if (mode == ECursorLock::NONE)
	{
		glfwSetInputMode(gameWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else if (mode == ECursorLock::LOCKED)
	{
		glfwSetInputMode(gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}
