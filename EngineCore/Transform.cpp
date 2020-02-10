#include "Transform.h"

using namespace DirectX;

Transform::Transform() : Component("Transform")
{
	position = { 0.f, 0.f, 0.f };
	eulerAngle = { 0.f, 0.f, 0.f };
	rotation = { 0.f, 0.f, 0.f, 0.f };
	scale = { 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());

	forward = { 0.f, 0.f, 1.f };
	backward = { 0.f, 0.f, -1.f };
	up = { 0.f, 1.f, 0.f };
	down = { 0.f, -1.f, 0.f };
	left = { -1.f, 0.f, 0.f };
	right = { 1.f, 0.f, 0.f };
	dirty = true;
}


Transform::~Transform()
{
}


XMFLOAT3 Transform::Position() const
{
	return position;
}


void Transform::Position(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
	dirty = true;
}


void Transform::Position(XMFLOAT3 position)
{
	this->position = position;
	dirty = true;
}


XMFLOAT4 Transform::Rotation() const
{
	return rotation;
}


XMFLOAT3 Transform::EulerAngles()
{
	eulerAngle = { fmodf(eulerAngle.x, 360.f), fmodf(eulerAngle.y, 360.f), fmodf(eulerAngle.z, 360.f) };

	return eulerAngle;
}


void Transform::Rotation(float x, float y, float z, float w)
{
	Rotation({ x, y, z, w });
}


void Transform::Rotation(XMFLOAT4 rotation)
{
	this->rotation = rotation;

	XMVECTOR quat = XMLoadFloat4(&rotation);
	XMVECTOR angleAxis;
	float angle;
	XMQuaternionToAxisAngle(&angleAxis, &angle, quat);
	XMStoreFloat3(&eulerAngle, XMVectorScale(angleAxis, angle));

	dirty = true;
}


void Transform::Rotate(float x, float y, float z)
{
	Rotate({ x, y, z });
}


void Transform::Rotate(XMFLOAT3 rotate)
{
	XMVECTOR newRot = XMVectorAdd(XMLoadFloat3(&eulerAngle), XMLoadFloat3(&rotate));
	XMStoreFloat3(&eulerAngle, newRot);

	newRot = XMVectorScale(newRot, XM_PI / 180);
	XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(newRot);

	XMStoreFloat4(&rotation, quat);
	dirty = true;
}


void Transform::EulerRotation(float x, float y, float z)
{
	EulerRotation({ x, y, z });
}


void Transform::EulerRotation(XMFLOAT3 eulerRotation)
{
	XMVECTOR rot = { XMConvertToRadians(eulerRotation.x), XMConvertToRadians(eulerRotation.y) , XMConvertToRadians(eulerRotation.z) };
	XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(rot);

	XMStoreFloat4(&rotation, quat);
	eulerAngle = eulerRotation;

	dirty = true;
}


XMFLOAT3 Transform::Scale() const
{
	return scale;
}


void Transform::Scale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);
	dirty = true;
}


void Transform::Scale(XMFLOAT3 scale)
{
	this->scale = scale;
	dirty = true;
}


XMFLOAT3 Transform::Forward()
{
	UpdateTransform();
	return forward;
}


XMFLOAT3 Transform::Backward()
{
	UpdateTransform();
	return backward;
}


XMFLOAT3 Transform::Up()
{
	UpdateTransform();
	return up;
}


XMFLOAT3 Transform::Down()
{
	UpdateTransform();
	return down;
}


XMFLOAT3 Transform::Left()
{
	UpdateTransform();
	return left;
}


XMFLOAT3 Transform::Right()
{
	UpdateTransform();
	return right;
}


void Transform::UpdateDirectionalVectors()
{
	XMMATRIX mat = XMLoadFloat4x4(&worldMatrix);
	XMMATRIX rotationMat = XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));

	XMVECTOR forwardVec = { 0.f, 0.f, 1.f };
	XMVECTOR forwardTransform = XMVector4Transform(forwardVec, rotationMat);
	XMStoreFloat3(&forward, forwardTransform);
	XMStoreFloat3(&backward, XMVectorScale(forwardTransform, -1.f));

	XMVECTOR upVec = { 0.f, 1.f, 0.f };
	XMVECTOR upTransform = XMVector4Transform(upVec, rotationMat);
	XMStoreFloat3(&up, upTransform);
	XMStoreFloat3(&down, XMVectorScale(upTransform, -1.f));

	XMVECTOR rightVec = { 1.f, 0.f, 0.f };
	XMVECTOR rightTransform = XMVector4Transform(rightVec, rotationMat);
	XMStoreFloat3(&right, rightTransform);
	XMStoreFloat3(&left, XMVectorScale(rightTransform, -1.f));
}


XMFLOAT4X4 Transform::WorldMatrix()
{
	UpdateTransform();
	return worldMatrix;
}


void Transform::UpdateWorldMatrix()
{
	XMMATRIX scaleMat = XMMatrixScalingFromVector(XMLoadFloat3(&scale));
	XMMATRIX rotationMat = XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));
	XMMATRIX positionMat = XMMatrixTranslationFromVector(XMLoadFloat3(&position));

	XMMATRIX worldMat = XMMatrixIdentity() * scaleMat * rotationMat * positionMat;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(worldMat));
}


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
