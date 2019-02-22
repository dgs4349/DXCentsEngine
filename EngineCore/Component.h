#pragma once

#include "Object.h"

class GameObject;
class Transform;

class Component : public Object
{
	friend class GameObject;
public:

	GameObject* gameObject;
	Transform* transform;

	Component();

protected:

	Component(std::string name);
	virtual ~Component() override;

	template <class T>
	void DestroyComponent();
};

template<class T>
inline void Component::DestroyComponent()
{
	gameObject->RemoveComponent<T>();
}
