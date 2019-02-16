#pragma once

#include <string>
#include "Object.h"
#include "GameObject.h"

template <class T>
class Component : public Object
{
	friend class GameObject;

public:

	/// <summary>
	/// The game object this component is attached to
	/// </summary>
	GameObject* gameObject = NULL;

	Component() {}


protected:
	/// <summary>
	/// Naming constructor
	/// </summary>
	/// <param name="name">The name of the component</param>
	Component(std::string name) : Object(name) {}
	~Component() override {}
};
