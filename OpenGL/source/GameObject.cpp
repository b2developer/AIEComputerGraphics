#include "GameObject.h"
#include "Transform.h"

//constructor
GameObject::GameObject()
{
	Transform* t = new Transform(vec3(0,0,0), vec3(0,0,1), vec3(0,1,0), vec3(1,1,1));
	t->gameObject = this;
	transform = t;

	components.push_back(t);
}

//destructor
GameObject::~GameObject()
{
	//iterate through all components, removing each
	for (vector<Component*>::iterator iter = components.begin(); iter != components.end(); iter++)
	{
		delete *iter;
	}
}

//main update loop
void GameObject::update(float deltaTime)
{
	//iterate through all components, updating each
	for (vector<Component*>::iterator iter = components.begin(); iter != components.end(); iter++)
	{
		(*iter)->update(deltaTime);
	}
}

//main render loop
void GameObject::draw(mat4 viewProjectionMatrix)
{
	//iterate through all components, drawing each
	for (vector<Component*>::iterator iter = components.begin(); iter != components.end(); iter++)
	{
		(*iter)->draw(viewProjectionMatrix);
	}
}
