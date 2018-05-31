#pragma once
#include <GLFW/glfw3.h>

#define INP Input::getInstance()

const int MAX_KEY_SIZE = 256;

enum EKeyInput
{
	RELEASE,
	PRESS,
};

enum ECursorLock
{
	NONE,
	LOCKED,
};

/*
* class Input
*
* singleton that serves as a container for information regarding user input 
* eg. (mouse movement, buttons and keystrokes)
* callbacks must be setup by an application first
*
* @author: Bradley Booth, Academy of Interactive Entertainment, 2018
*/
class Input
{
private:
	
	/*
	* Input()
	* default constructor
	*/
	Input() {}

	/*
	* Input()
	* default destructor
	*/
	~Input() {}

public: 

	//pointer to the window
	GLFWwindow * gameWindow;

	//input data
	double mouseX = 0.0;
	double mouseY = 0.0;
	double mouseMovementX = 0.0;
	double mouseMovementY = 0.0;

	EKeyInput keyStates[MAX_KEY_SIZE];
	ECursorLock cursorMode = ECursorLock::NONE;

	/*
	* getInstance
	*
	* gets the only instance of the singleton, creates it
	* if this is the first time this function is being invoked
	* 
	* @returns static Input* - the singleton instance
	*/
	static Input* getInstance();

	/*
	* deltaReset
	*
	* sets all delta-variables to zero, ensures that the callback
	* system still works when no callbacks are being made
	*
	* @returns void
	*/
	void deltaReset();

	/*
	* setCursorLockMode
	*
	* sets the cursor to either be locked or unlocked from the window
	*
	* @param ECursorLock mode - the new lock mode of the cursor
	* @returns void
	*/
	void setCursorLockMode(ECursorLock mode);
};