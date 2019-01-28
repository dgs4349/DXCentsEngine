#include "Entity.h"

Entity::Entity(Mesh* const mesh, Material* const material)
{
	this->mesh = mesh;
	transform.Position(0.0f, 0.0f, 0.0f);
	transform.Rotation(0.0f, 0.0f, 0.0f, 0.0f);
	transform.Scale(1.0f, 1.0f, 1.0f);

	this->material = material;
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

void Entity::PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	material->VertexShader()->SetMatrix4x4("view", viewMatrix);
	material->VertexShader()->SetMatrix4x4("projection", projectionMatrix);
	material->VertexShader()->SetMatrix4x4("world", transform.WorldMatrix());

	material->VertexShader()->SetShader();
	material->PixelShader()->SetShader();

	material->VertexShader()->CopyAllBufferData();
}