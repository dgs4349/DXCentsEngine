#include "Transform.h"

namespace Component
{
	Transform::Transform()
	{
		position = { 0.0f, 0.0f, 0.0f };
		eulerAngle = { 0.0f, 0.0f, 0.0f };
		rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
		scale = { 1.0f, 1.0f, 1.0f };
		DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());

		forward = { 0.0f, 0.0f, 1.0f };
		backward = { 0.0f, 0.0f, -1.0f };
		up = { 0.0f, 1.0f, 0.0f };
		down = { 0.0f, -1.0f, 0.0f };
		left = { -1.0f, 0.0f, 0.0f };
		right = { 1.0f, 0.0f, 0.0f };
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

	DirectX::XMFLOAT4 Transform::Rotation() const
	{
		return rotation;
	}

	DirectX::XMFLOAT3 Transform::EulerAngles() const
	{
		return eulerAngle;
	}

	void Transform::Rotation(float x, float y, float z, float w)
	{
		rotation = DirectX::XMFLOAT4(x, y, z, w);
		dirty = true;
	}

	void Transform::Rotation(DirectX::XMFLOAT4 rotation)
	{
		this->rotation = rotation;
		dirty = true;
	}

	void Transform::Rotate(float x, float y, float z)
	{
		DirectX::XMVECTOR newRot = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&eulerAngle), { x, y, z });
		DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYawFromVector(newRot);

		DirectX::XMStoreFloat4(&rotation, quat);
		DirectX::XMStoreFloat3(&eulerAngle, newRot);
		dirty = true;
	}

	void Transform::Rotate(DirectX::XMFLOAT3 rotate)
	{
		DirectX::XMVECTOR newRot = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&eulerAngle), DirectX::XMLoadFloat3(&rotate));
		DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYawFromVector(newRot);

		DirectX::XMStoreFloat4(&rotation, quat);
		DirectX::XMStoreFloat3(&eulerAngle, newRot);
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

	DirectX::XMFLOAT3 Transform::Backward()
	{
		UpdateTransform();
		return backward;
	}

	DirectX::XMFLOAT3 Transform::Up()
	{
		UpdateTransform();
		return up;
	}

	DirectX::XMFLOAT3 Transform::Down()
	{
		UpdateTransform();
		return down;
	}

	DirectX::XMFLOAT3 Transform::Left()
	{
		UpdateTransform();
		return left;
	}

	DirectX::XMFLOAT3 Transform::Right()
	{
		UpdateTransform();
		return right;
	}

	void Transform::UpdateDirectionalVectors()
	{
		DirectX::XMMATRIX mat = DirectX::XMLoadFloat4x4(&worldMatrix);
		DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));

		DirectX::XMVECTOR forwardVec = { 0.0f, 0.0f, 1.0f };
		DirectX::XMVECTOR forwardTransform = DirectX::XMVector4Transform(forwardVec, rotationMat);
		DirectX::XMStoreFloat3(&forward, forwardTransform);
		DirectX::XMStoreFloat3(&backward, DirectX::XMVectorScale(forwardTransform, -1.0f));

		DirectX::XMVECTOR upVec = { 0.0f, 1.0f, 0.0f };
		DirectX::XMVECTOR upTransform = DirectX::XMVector4Transform(upVec, rotationMat);
		DirectX::XMStoreFloat3(&up, upTransform);
		DirectX::XMStoreFloat3(&down, DirectX::XMVectorScale(upTransform, -1.0f));

		DirectX::XMVECTOR rightVec = { 1.0f, 0.0f, 0.0f };
		DirectX::XMVECTOR rightTransform = DirectX::XMVector4Transform(rightVec, rotationMat);
		DirectX::XMStoreFloat3(&right, rightTransform);
		DirectX::XMStoreFloat3(&left, DirectX::XMVectorScale(rightTransform, -1.0f));
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
		DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scale));
		DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
		DirectX::XMMATRIX positionMat = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position));

		DirectX::XMMATRIX worldMat = DirectX::XMMatrixIdentity() * scaleMat * rotationMat * positionMat;
		DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixTranspose(worldMat));
	}

#pragma endregion

	void Transform::SetDirty()
	{
		dirty = true;
	}

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