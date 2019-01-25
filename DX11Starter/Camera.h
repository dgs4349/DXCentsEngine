#pragma once

#include <d3d11.h>
#include "Transform.h"

class Camera
{
public:

	Component::Transform transform;

	Camera();
	~Camera();

	DirectX::XMFLOAT4X4 ViewMatrix();

	void Update(float deltatime);

private:

	DirectX::XMFLOAT4X4 viewMatrix;

	void UpdateViewMatrix();
};

