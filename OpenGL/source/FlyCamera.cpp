#include "FlyCamera.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"
#include <iostream>

//main update loop
void FlyCamera::update(float deltaTime)
{
	float PI = glm::pi<float>();
	float DEG2RAD = 180.0f / PI;

	vec3 input = vec3(0, 0, 0);

	//only get input if the window has locked the mouse
	if (INP->cursorMode == ECursorLock::LOCKED)
	{
		//change in mouse position
		float dmx = (float)INP->mouseMovementX;
		float dmy = (float)INP->mouseMovementY;

		rotationAngles += vec2(-dmx, dmy) * deltaTime * sensitivity / DEG2RAD;
		
		rotationAngles.y = glm::clamp(rotationAngles.y, -PI / 2.0f + 0.001f, PI / 2.0f - 0.001f);

		float xsn = sinf(rotationAngles.x);
		float xcs = cosf(rotationAngles.x);

		float ysn = sinf(rotationAngles.y);
		float ycs = cosf(rotationAngles.y);

		vec2 ref = vec2(0, 1.0);

		vec2 ver = ref;

		//vertical rotation
		ver.x = ref.x * ycs - ref.y * ysn;
		ver.y = ref.x * ysn + ref.y * ycs;

		vec2 hor = vec2(ver.y, 0.0);

		//horizontal rotation
		hor.x = ver.y * xcs;
		hor.y = ver.y * xsn;

		vec3 f = vec3(hor.y, ver.x, hor.x);
		gameObject->transform->forward = f;

		//get the input variables
		if (INP->keyStates[forwardCode] == EButtonInput::PRESS)
		{
			input.z += 1.0f;
		}

		if (INP->keyStates[backwardCode] == EButtonInput::PRESS)
		{
			input.z -= 1.0f;
		}

		if (INP->keyStates[leftCode] == EButtonInput::PRESS)
		{
			input.x += 1.0f;
		}

		if (INP->keyStates[rightCode] == EButtonInput::PRESS)
		{
			input.x -= 1.0f;
		}

		if (INP->keyStates[upCode] == EButtonInput::PRESS)
		{
			input.y += 1.0f;
		}

		if (INP->keyStates[downCode] == EButtonInput::PRESS)
		{
			input.y -= 1.0f;
		}

		//only normalise a non-zero vector
		if (input != vec3(0, 0, 0))
		{
			glm::normalize(input);
		}

		vec2 horInput = vec2(input.x, input.z);
		
		//rotate the input vector to match the looking direction
		horInput.x = xcs * input.x + xsn * input.z;
		horInput.y = -xsn * input.x + xcs * input.z;

		input.x = horInput.x;
		input.z = horInput.y;

		//unlock the mouse
		if (INP->keyStates[escapeCode] == EButtonInput::PRESS)
		{
			INP->setCursorLockMode(ECursorLock::NONE);
		}
	}
	//lock the mouse
	else if (INP->buttonStates[attackCode] == EButtonInput::PRESS)
	{
		INP->setCursorLockMode(ECursorLock::LOCKED);
	}

	velocity += input * acceleration * deltaTime;

	//apply friction scaled by deltaTime
	velocity *= glm::pow(friction, deltaTime);

	float sqrLen = velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z;

	//clamp velocity
	if (sqrLen > maxSpeed * maxSpeed)
	{
		float ratio = maxSpeed / glm::length(velocity);
		velocity *= ratio;
	}
	
	gameObject->transform->position += velocity * deltaTime;
	gameObject->transform->onTransformUpdate();
	
}