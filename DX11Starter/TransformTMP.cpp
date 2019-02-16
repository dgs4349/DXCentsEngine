#include "..\EngineCore\Transform.h"
#include "TransformTMP.h"

using namespace DirectX;

namespace Component
{
	TransformTMP::TransformTMP()
	{
		position = { 0.0f, 0.0f, 0.0f };
		eulerAngle = { 0.0f, 0.0f, 0.0f };
		rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
		scale = { 1.0f, 1.0f, 1.0f };
		XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());

		forward = { 0.0f, 0.0f, 1.0f };
		backward = { 0.0f, 0.0f, -1.0f };
		up = { 0.0f, 1.0f, 0.0f };
		down = { 0.0f, -1.0f, 0.0f };
		left = { -1.0f, 0.0f, 0.0f };
		right = { 1.0f, 0.0f, 0.0f };
		dirty = true;
	}

#pragma region Position Getter/Setter Functions

	XMFLOAT3 TransformTMP::Position() const
	{
		return position;
	}

	void TransformTMP::Position(float x, float y, float z)
	{
		position = XMFLOAT3(x, y, z);
		dirty = true;
	}

	void TransformTMP::Position(XMFLOAT3 position)
	{
		this->position = position;
		dirty = true;
	}

#pragma endregion

#pragma region Rotation Getter/Setter Functions

	XMFLOAT4 TransformTMP::Rotation() const
	{
		return rotation;
	}

	XMFLOAT3 TransformTMP::EulerAngles() const
	{
		return eulerAngle;
	}

	void TransformTMP::Rotation(float x, float y, float z, float w)
	{
		rotation = XMFLOAT4(x, y, z, w);
		dirty = true;
	}

	void TransformTMP::Rotation(XMFLOAT4 rotation)
	{
		this->rotation = rotation;
		dirty = true;
	}

	void TransformTMP::Rotate(float x, float y, float z)
	{
		XMVECTOR newRot = XMVectorAdd(XMLoadFloat3(&eulerAngle), { x, y, z });
		XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(newRot);

		XMStoreFloat4(&rotation, quat);
		XMStoreFloat3(&eulerAngle, newRot);
		dirty = true;
	}

	void TransformTMP::Rotate(XMFLOAT3 rotate)
	{
		XMVECTOR newRot = XMVectorAdd(XMLoadFloat3(&eulerAngle), XMLoadFloat3(&rotate));
		XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(newRot);

		XMStoreFloat4(&rotation, quat);
		XMStoreFloat3(&eulerAngle, newRot);
		dirty = true;
	}

#pragma endregion

#pragma region Scale Getter/Setter Functions

	XMFLOAT3 TransformTMP::Scale() const
	{
		return scale;
	}

	void TransformTMP::Scale(float x, float y, float z)
	{
		scale = XMFLOAT3(x, y, z);
		dirty = true;
	}

	void TransformTMP::Scale(XMFLOAT3 scale)
	{
		this->scale = scale;
		dirty = true;
	}

#pragma endregion

#pragma region Directional Getter/Update Functions

	XMFLOAT3 TransformTMP::Forward()
	{
		UpdateTransform();
		return forward;
	}

	XMFLOAT3 TransformTMP::Backward()
	{
		UpdateTransform();
		return backward;
	}

	XMFLOAT3 TransformTMP::Up()
	{
		UpdateTransform();
		return up;
	}

	XMFLOAT3 TransformTMP::Down()
	{
		UpdateTransform();
		return down;
	}

	XMFLOAT3 TransformTMP::Left()
	{
		UpdateTransform();
		return left;
	}

	XMFLOAT3 TransformTMP::Right()
	{
		UpdateTransform();
		return right;
	}

	void TransformTMP::UpdateDirectionalVectors()
	{
		XMMATRIX mat = XMLoadFloat4x4(&worldMatrix);
		XMMATRIX rotationMat = XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));

		XMVECTOR forwardVec = { 0.0f, 0.0f, 1.0f };
		XMVECTOR forwardTransform = XMVector4Transform(forwardVec, rotationMat);
		XMStoreFloat3(&forward, forwardTransform);
		XMStoreFloat3(&backward, XMVectorScale(forwardTransform, -1.0f));

		XMVECTOR upVec = { 0.0f, 1.0f, 0.0f };
		XMVECTOR upTransform = XMVector4Transform(upVec, rotationMat);
		XMStoreFloat3(&up, upTransform);
		XMStoreFloat3(&down, XMVectorScale(upTransform, -1.0f));

		XMVECTOR rightVec = { 1.0f, 0.0f, 0.0f };
		XMVECTOR rightTransform = XMVector4Transform(rightVec, rotationMat);
		XMStoreFloat3(&right, rightTransform);
		XMStoreFloat3(&left, XMVectorScale(rightTransform, -1.0f));
	}

#pragma endregion

#pragma region World Matrix Getter/Updater Functions

	XMFLOAT4X4 TransformTMP::WorldMatrix()
	{
		UpdateTransform();
		return worldMatrix;
	}

	void TransformTMP::UpdateWorldMatrix()
	{
		XMMATRIX scaleMat = XMMatrixScalingFromVector(XMLoadFloat3(&scale));
		XMMATRIX rotationMat = XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));
		XMMATRIX positionMat = XMMatrixTranslationFromVector(XMLoadFloat3(&position));

		XMMATRIX worldMat = XMMatrixIdentity() * scaleMat * rotationMat * positionMat;
		XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(worldMat));
	}

#pragma endregion

	void TransformTMP::SetDirty()
	{
		dirty = true;
	}

	void TransformTMP::UpdateTransform()
	{
		if (dirty)
		{
			UpdateWorldMatrix();
			UpdateDirectionalVectors();
			dirty = false;
		}
	}
}