#include "Camera.h"

Camera::Camera()
{
	DirectX::XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixIdentity());
}


Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	DirectX::XMVECTOR move = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 movement = { 0.0f, 0.0f, 0.0f };

	if (GetAsyncKeyState('R') & 0x8000)
	{
		transform.Rotate(0.1f, 0.0f, 0.0f);
	}

	if (GetAsyncKeyState('W') & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Forward()));
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Backward()));
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Left()));
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Right()));
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Up()));
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Down()));
	}

	move = DirectX::XMVectorAdd(
		DirectX::XMLoadFloat3(&transform.Position()),
		DirectX::XMVectorScale(move, deltaTime * 5.0f)
	);
	DirectX::XMStoreFloat3(&movement, move);
	transform.Position(movement);
}

DirectX::XMFLOAT4X4 Camera::ViewMatrix()
{
	UpdateViewMatrix();
	return viewMatrix;
}

void Camera::UpdateViewMatrix()
{
	transform.SetDirty();

	DirectX::XMStoreFloat4x4(&viewMatrix,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixLookToLH(
				DirectX::XMLoadFloat3(&transform.Position()),
				DirectX::XMLoadFloat3(&transform.Forward()),
				DirectX::XMLoadFloat3(&transform.Up())
			)
		)
	);
}

#pragma region Screen Size Functions

void Camera::SetScreenSize(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	UpdateProjectionMatrix();
}

void Camera::SetScreenSize(DirectX::XMFLOAT2 size)
{
	screenWidth = size.x;
	screenHeight = size.y;
	UpdateProjectionMatrix();
}

DirectX::XMFLOAT2 Camera::ScreenSize() const
{
	return DirectX::XMFLOAT2(screenWidth, screenHeight);
}

int Camera::ScreenWidth() const
{
	return screenWidth;
}

int Camera::ScreenHeight() const
{
	return screenHeight;
}

#pragma endregion

#pragma region Projection Functions
DirectX::XMFLOAT4X4 Camera::ProjectionMatrix()
{
	UpdateProjectionMatrix();
	return projectionMatrix;
}

void Camera::OnResize()
{
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,
		(float)screenWidth / screenHeight,
		0.1f,
		100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}

void Camera::UpdateProjectionMatrix()
{
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,
		(float)(screenWidth / screenHeight),
		0.1f,
		100.0f);
	DirectX::XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}
#pragma endregion