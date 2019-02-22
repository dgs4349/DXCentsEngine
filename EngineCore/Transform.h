#pragma once

#include "Component.h"


class Transform : public Component
{
public:

	Transform();
	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="transform">The transform to copy</param>
	Transform(Transform& transform);

protected:
	~Transform() override;
};

