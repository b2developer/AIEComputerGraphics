#include "Renderable.h"
#include "GameObject.h"
#include "Transform.h"

//main update loop
void Renderable::update(float deltaTime)
{
	//don't spin unless told to
	if (!spin)
	{
		return;
	}

	spinAngle += spinSpeed * deltaTime;

	float sn = sinf(spinAngle);
	float cs = cosf(spinAngle);

	vec2 ref = vec2(0, 1.0);
	
	vec2 ver = ref;

	//vertical rotation
	ver.x = ref.x * cs - ref.y * sn;
	ver.y = ref.x * sn + ref.y * cs;

	gameObject->transform->forward = vec3(ver.y, 0.0f, ver.x);
	gameObject->transform->onTransformUpdate();
}
