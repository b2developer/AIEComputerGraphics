#pragma once
#include <vector>

using namespace std;

//forward declaration
class Component;
class Transform;

class GameObject
{
public:

	//list of components contained by the game-object
	vector<Component*> components;

	GameObject();
	~GameObject();
};