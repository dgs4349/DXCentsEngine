#include "GameObject.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"

GameObject::GameObject() : Object("GameObject")
{
	transform = AddComponent<Transform>();
}


GameObject::GameObject(std::string name) : Object(name)
{
	transform = AddComponent<Transform>();
}

GameObject::GameObject(std::string name, Mesh* mesh, Material* material)
{
	transform = AddComponent<Transform>();
	AddComponent<MeshFilter>(mesh);
	AddComponent<MeshRenderer>(material);
}


GameObject::~GameObject()
{
}
