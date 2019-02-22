#pragma once

#include "Object.h"

class GameObject;
class Transform;

class Component : public Object
{
	friend class GameObject;
public:

	GameObject* gameObject;
	Transform* transform;

	Component();

protected:

	Component(std::string name);
	virtual ~Component() override;
};
