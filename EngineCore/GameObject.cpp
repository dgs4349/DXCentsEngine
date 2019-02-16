#include "GameObject.h"

#include "Logger.h"

GameObject::GameObject() : Object("GameObject")
{
	transform = new Transform();
}

GameObject::GameObject(std::string name) : Object(name)
{
	transform = new Transform(const_cast<GameObject*>(this));
}

void GameObject::AddComponent()
{
}

GameObject::~GameObject()
{
}
