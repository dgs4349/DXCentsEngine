#pragma once

#include "Component.h"

class Transform : public Component<Transform>
{
	friend class GameObject;

public:

	/// <summary>
	/// Naming constructor
	/// </summary>
	Transform();


protected:

	/// <summary>
	/// Game object constructor
	/// </summary>
	/// <param name="object">The gameobject this component is attached to</param>
	Transform(GameObject* object);
	~Transform() override;
};

