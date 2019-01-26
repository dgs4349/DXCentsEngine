#pragma once

#include <d3d11.h>
#include "Transform.h"

class Camera
{
public:

	Component::Transform transform;

	Camera();
	~Camera();

	void SetScreenSize(int width, int height);
	void SetScreenSize(DirectX::XMFLOAT2 size);
	DirectX::XMFLOAT2 ScreenSize() const;
	int ScreenWidth() const;
	int ScreenHeight() const;

	DirectX::XMFLOAT4X4 ViewMatrix();
	DirectX::XMFLOAT4X4 ProjectionMatrix();

	void OnResize();

	void Update(float deltatime);

private:

	uint16_t screenWidth;
	uint16_t screenHeight;

	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
};

