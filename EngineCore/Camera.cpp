#include "Camera.h"
#include "Transform.h"

using namespace DirectX;

Camera::Camera() : Component("Camera")
{
	XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());
}


Camera::~Camera()
{
}


void Camera::Update(float deltaTime)
{
	XMVECTOR move = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 movement = { 0.0f, 0.0f, 0.0f };

	if (GetAsyncKeyState('W') & 0x8000)
	{
		move = XMVectorAdd(move, XMLoadFloat3(&transform->Forward()));
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		move = XMVectorAdd(move, XMLoadFloat3(&transform->Backward()));
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		move = XMVectorAdd(move, XMLoadFloat3(&transform->Left()));
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		move = XMVectorAdd(move, XMLoadFloat3(&transform->Right()));
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		move = XMVectorAdd(move, XMLoadFloat3(&transform->Up()));
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		move = XMVectorAdd(move, XMLoadFloat3(&transform->Down()));
	}

	move = XMVectorAdd(
		XMLoadFloat3(&transform->Position()),
		XMVectorScale(move, deltaTime * 5.0f)
	);
	XMStoreFloat3(&movement, move);
	transform->Position(movement);
}


XMFLOAT4X4 Camera::ViewMatrix()
{
	UpdateViewMatrix();
	return viewMatrix;
}


void Camera::UpdateViewMatrix()
{
	transform->SetDirty();

	XMStoreFloat4x4(&viewMatrix,
		XMMatrixTranspose(
			XMMatrixLookToLH(
				XMLoadFloat3(&transform->Position()),
				XMLoadFloat3(&transform->Forward()),
				XMLoadFloat3(&transform->Up())
			)
		)
	);
}


void Camera::SetScreenSize(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	UpdateProjectionMatrix();
}


void Camera::SetScreenSize(XMFLOAT2 size)
{
	screenWidth = static_cast<uint16_t>(size.x);
	screenHeight = static_cast<uint16_t>(size.y);
	UpdateProjectionMatrix();
}


XMFLOAT2 Camera::ScreenSize() const
{
	return XMFLOAT2(screenWidth, screenHeight);
}


int Camera::ScreenWidth() const
{
	return screenWidth;
}


int Camera::ScreenHeight() const
{
	return screenHeight;
}


XMFLOAT4X4 Camera::ProjectionMatrix()
{
	UpdateProjectionMatrix();
	return projectionMatrix;
}


void Camera::OnResize()
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,
		(float)screenWidth / screenHeight,
		0.1f,
		100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}


void Camera::UpdateProjectionMatrix()
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,
		(float)screenWidth / screenHeight,
		0.1f,
		100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}
