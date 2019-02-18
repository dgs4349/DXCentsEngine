#pragma once

#include "Object.h"

class GameObject;
class Transform;

class Component : public Object
{
public:

	GameObject* gameObject;
	Transform* transform;

	Component();

protected:

	Component(std::string name);
	~Component() override;
};

