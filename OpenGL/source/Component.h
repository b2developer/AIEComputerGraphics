#pragma once

//forward declaration
class GameObject;

class Component
{
public:

	GameObject * gameObject;

	Component();
	virtual ~Component();
};