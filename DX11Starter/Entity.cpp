#include "Entity.h"



Entity::Entity(Mesh* const mesh)
{
	this->mesh = mesh;
	transform.Position(0.0f, 0.0f, 0.0f);
	transform.Rotation(0.0f, 0.0f, 0.0f);
	transform.Scale(1.0f, 1.0f, 1.0f);
}


Entity::~Entity()
{
}

Mesh* const Entity::GetMesh() const
{
	return mesh;
}

void Entity::SetMesh(Mesh* const mesh)
{
	this->mesh = mesh;
}