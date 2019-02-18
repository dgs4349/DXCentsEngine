#include "GameObject.h"

GameObject::GameObject() : Object("GameObject")
{
	transform = AddComponent<Transform>();
}


GameObject::GameObject(std::string name) : Object(name)
{
	transform = AddComponent<Transform>();
}


GameObject::~GameObject()
{
}
