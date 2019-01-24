#include "Transform.h"

namespace Component
{
	Transform::Transform()
	{
		position = { 0.0f, 0.0f, 0.0f };
		rotation = { 0.0f, 0.0f, 0.0f };
		scale = { 1.0f, 1.0f, 1.0f };
		DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
		dirty = true;
	}

#pragma region Position Getter/Setter Functions

	DirectX::XMFLOAT3 Transform::Position() const
	{
		return position;
	}

	void Transform::Position(float x, float y, float z)
	{
		position = DirectX::XMFLOAT3(x, y, z);
		dirty = true;
	}

	void Transform::Position(DirectX::XMFLOAT3 position)
	{
		this->position = position;
		dirty = true;
	}

#pragma endregion

#pragma region Rotation Getter/Setter Functions

	DirectX::XMFLOAT3 Transform::Rotation() const
	{
		return rotation;
	}

	void Transform::Rotation(float x, float y, float z)
	{
		rotation = DirectX::XMFLOAT3(x, y, z);
		dirty = true;
	}

	void Transform::Rotation(DirectX::XMFLOAT3 rotation)
	{
		this->scale = scale;
		dirty = true;
	}

#pragma endregion

#pragma region Scale Getter/Setter Functions

	DirectX::XMFLOAT3 Transform::Scale() const
	{
		return scale;
	}

	void Transform::Scale(float x, float y, float z)
	{
		scale = DirectX::XMFLOAT3(x, y, z);
		dirty = true;
	}

	void Transform::Scale(DirectX::XMFLOAT3 scale)
	{
		this->scale = scale;
		dirty = true;
	}

#pragma endregion

#pragma region Directional Getter/Update Functions

	DirectX::XMFLOAT3 Transform::Forward()
	{
		UpdateTransform();
		return forward;
	}

	DirectX::XMFLOAT3 Transform::Up()
	{
		UpdateTransform();
		return up;
	}

	DirectX::XMFLOAT3 Transform::Left()
	{
		UpdateTransform();
		return left;
	}

	void Transform::UpdateDirectionalVectors()
	{
		DirectX::XMMATRIX mat = DirectX::XMLoadFloat4x4(&worldMatrix);

		DirectX::XMStoreFloat3(&forward, DirectX::XMVector3Normalize(mat.r[2]));
		DirectX::XMStoreFloat3(&up, DirectX::XMVector3Normalize(mat.r[1]));
		DirectX::XMStoreFloat3(&left, DirectX::XMVector3Normalize(mat.r[0]));
	}

#pragma endregion

#pragma region World Matrix Getter/Updater Functions

	DirectX::XMFLOAT4X4 Transform::WorldMatrix()
	{
		UpdateTransform();
		return worldMatrix;
	}

	void Transform::UpdateWorldMatrix()
	{
		DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		DirectX::XMMATRIX positionMat = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

		DirectX::XMMATRIX worldMat = scaleMat * rotationMat * positionMat;
		DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixTranspose(worldMat));
	}

#pragma endregion

	void Transform::UpdateTransform()
	{
		if (dirty)
		{
			UpdateWorldMatrix();
			UpdateDirectionalVectors();
			dirty = false;
		}
	}
}