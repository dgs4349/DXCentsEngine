#pragma once
#include <string>

#include "Object.h"
#include "Transform.h"
#include "stdHelper.h"

class GameObject : public Object
{
public:

	Transform* transform;

	GameObject();
	GameObject(std::string name);

	static GameObject* newGameObject()
	{
		return new GameObject();
	}

	template <class T, class ...ARGS>
	T* AddComponent(ARGS&&... args)
	{
		if (!std::is_base_of<Component, T>::value)
		{
			LOG_TRACE("Tried to add {} which is not a component, returning", GetTypeName<T>());
			return nullptr;
		}

		T* component = GetComponent<T>();
		if (component != nullptr)
		{
			LOG_TRACE("Tried to add {} which already is attached to GameObject, returning", GetTypeName<T>());
			return nullptr;
		}

		component = new T(std::forward<ARGS>(args)...);
		reinterpret_cast<Component*>(component)->gameObject = this;
		attachedComponents.insert({ GetTypeName<T>(), reinterpret_cast<Component*>(component) });

		return component;
	}

	template <class T>
	T* GetComponent()
	{
		if (!std::is_base_of<Component, T>::value)
		{
			LOG_TRACE("Tried to find {} which is not a component, returning", GetTypeName<T>());
			return nullptr;
		}

		std::unordered_map<std::string, Component*>::iterator component = attachedComponents.find(GetTypeName<T>());

		if (component == attachedComponents.end())
		{
			return nullptr;
		}
		else
		{
			return reinterpret_cast<T*>(component->second);
		}
	}

	void RemoveComponent(Component* component)
	{
		if (static_cast<Transform*>(component) == transform)
		{
			LOG_TRACE("Tried to remove {} which cannot be removed from a GameObject, returning", component->name);
			return;
		}

		attachedComponents.erase(attachedComponents.find(component->name));
	}

protected:

	~GameObject();

	std::unordered_map<std::string, Component*> attachedComponents;
};