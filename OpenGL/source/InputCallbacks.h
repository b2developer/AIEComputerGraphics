#pragma once
#include <GLFW/glfw3.h>
#include "Input.h"

//all functions are outside of a class because they can't be function pointers otherwise
//this is essentially an extension wrapper of the Input singleton

/*
* onKeyModified
*
* GLFW automatically calls this call-back whenever a key's state is changed
* the application notes the new state of the key
*
* @param GLFWwindow* window - the window that recieved it
* @param int key - the key that was called
* @param int action - the action that the key currently is invoking
* @param int mods - any modifier bits
* @returns void
*/
void onKeyModified(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//unrecognised key
	if (key > MAX_KEY_SIZE - 1)
	{
		return;
	}

	//sticky-key filtering
	if (action == 2)
	{
		action = 1;
	}

	INP->keyStates[key] = (EButtonInput)action;
}

/*
* onMouseModified
*
* GLFW automatically calls this call-back whenever the mouse is moved
*
* @param GLFWwindow* window - the window that recieved it
* @param float xPos - the new position of the mouse
* @param float yPos - the new position of the mouse
* @returns void
*/
void onMouseModified(GLFWwindow* window, double xPos, double yPos)
{
	INP->mouseMovementX = xPos - INP->mouseX;
	INP->mouseMovementY = yPos - INP->mouseY;

	INP->mouseX = xPos;
	INP->mouseY = yPos;
}

/*
* onMouseButtonModified
*
* GLFW automatically calls this call-back whenever a mouse button changes state
*
* @param GLFWwindow* window - the window that recieved it
* @param int button - the button that was called
* @param int action - the action that the button currently is invoking
* @param int mods - any modifier bits
*/
void onMouseButtonModified(GLFWwindow* window, int button, int action, int mods)
{
	//unrecognised button
	if (button > MAX_MOUSE_BUTTON_SIZE - 1)
	{
		return;
	}

	INP->buttonStates[button] = (EButtonInput)action;
}
