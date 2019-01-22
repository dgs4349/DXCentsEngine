#include "Entity.h"



Entity::Entity(Mesh* const mesh)
{
	this->mesh = mesh;
	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f };
	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	dirty = true;
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

DirectX::XMFLOAT3 Entity::Position() const
{
	return position;
}

void Entity::Position(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
	dirty = true;
}

void Entity::Position(DirectX::XMFLOAT3 position)
{
	this->position = position;
	dirty = true;
}

DirectX::XMFLOAT3 Entity::Rotation() const
{
	return rotation;
}

void Entity::Rotation(float x, float y, float z)
{
	rotation = DirectX::XMFLOAT3(x, y, z);
	dirty = true;
}

void Entity::Rotation(DirectX::XMFLOAT3 rotation)
{
	this->rotation = rotation;
	dirty = true;
}

DirectX::XMFLOAT3 Entity::Scale() const
{
	return scale;
}

void Entity::Scale(float x, float y, float z)
{
	scale = DirectX::XMFLOAT3(x, y, z);
	dirty = true;
}

void Entity::Scale(DirectX::XMFLOAT3 scale)
{
	this->scale = scale;
	dirty = true;
}

DirectX::XMFLOAT4X4 Entity::WorldMatrix()
{
	if (dirty)
	{
		UpdateWorldMatrix();
		dirty = false;
	}

	return worldMatrix;
}

void Entity::UpdateWorldMatrix()
{
	DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	DirectX::XMMATRIX positionMat = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	DirectX::XMMATRIX worldMat = scaleMat * rotationMat * positionMat;
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixTranspose(worldMat));
}