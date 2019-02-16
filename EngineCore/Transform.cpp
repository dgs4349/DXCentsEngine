#include "Transform.h"


Transform::Transform() : Component("Transform")
{
}

Transform::Transform(GameObject* object) : Component("Transform")
{
	gameObject = object;
}

Transform::~Transform()
{
}
