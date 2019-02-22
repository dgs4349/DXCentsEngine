#pragma once

#include "Component.h"


class Transform : public Component
{
public:

	Transform();
	Transform(Transform& transform);

protected:
	~Transform() override;
};

