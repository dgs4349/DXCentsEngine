#pragma once

#include <string>
#include "Object.h"
#include "Transform.h"

class GameObject : public Object
{
	friend class Transform;

public:

	/// <summary>
	/// The tag of this game object
	/// </summary>
	std::string tag;
	/// <summary>
	/// The transform attached to this game object
	/// </summary>
	Transform* transform = NULL;

	/// <summary>
	/// Default constructor
	/// </summary>
	GameObject();
	/// <summary>
	/// Naming constructor
	/// </summary>
	/// <param name="name"></param>
	GameObject(std::string name);
	//GameObject(std::string name, std::vector<Component> components);

	void AddComponent();
	//void AddComponent();


protected:

	virtual ~GameObject() override;


private:

	/// <summary>
	/// Whether the game object is active in the scene
	/// </summary>
	bool activeInHierarchy = true;
	/// <summary>
	/// Whether the local state of this game object is active
	/// </summary>
	bool activeSelf = true;
};

