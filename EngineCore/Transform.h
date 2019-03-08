#pragma once

#include <DirectXMath.h>
#include "Component.h"

class Transform : public Component
{
public:

	Transform();

	DirectX::XMFLOAT3 Position() const;
	void Position(float x, float y, float z);
	void Position(DirectX::XMFLOAT3 position);

	DirectX::XMFLOAT4 Rotation() const;
	DirectX::XMFLOAT3 EulerAngles();
	void Rotation(float x, float y, float z, float w);
	void Rotation(DirectX::XMFLOAT4 rotation);
	void Rotate(float x, float y, float z);
	void Rotate(DirectX::XMFLOAT3 rotation);
	void EulerRotation(float x, float y, float z);
	void EulerRotation(DirectX::XMFLOAT3 eulerRotation);

	DirectX::XMFLOAT3 Scale() const;
	void Scale(float x, float y, float z);
	void Scale(DirectX::XMFLOAT3 scale);

	DirectX::XMFLOAT3 Forward();
	DirectX::XMFLOAT3 Backward();
	DirectX::XMFLOAT3 Up();
	DirectX::XMFLOAT3 Down();
	DirectX::XMFLOAT3 Left();
	DirectX::XMFLOAT3 Right();

	/// Get the world matrix of this Transform.
	DirectX::XMFLOAT4X4 WorldMatrix();

	void SetDirty();

protected:
	virtual ~Transform() override;

private:

	bool dirty;

	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 eulerAngle = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

	DirectX::XMFLOAT3 forward = { 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT3 backward = { 0.0f, 0.0f, -1.0f };
	DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
	DirectX::XMFLOAT3 down = { 0.0f, -1.0f, 0.0f };
	DirectX::XMFLOAT3 left = { -1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 right = { 1.0f, 0.0f, 0.0f };

	DirectX::XMFLOAT4X4 worldMatrix;

	void UpdateTransform();
	void UpdateWorldMatrix();
	void UpdateDirectionalVectors();
};

