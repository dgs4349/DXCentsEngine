#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "ObjectManager.h"

namespace HideFlags
{
	enum HideFlags
	{
		None,
	};

#define HideFlags HideFlags::HideFlags
}

/// <summary>
/// Base class for all objects that can be referenced
/// </summary>
class Object
{
	friend class ObjectManager;
public:

	/// <summary>
	/// The name of the object
	/// </summary>
	std::string name = "Object";
	/// <summary>
	/// Should the object be hidden, saved with the Scene, or modifiable by the user?
	/// </summary>
	HideFlags hideFlag = HideFlags::None;

	/// <summary>
	/// Create and register a new object
	/// </summary>
	Object();
	/// <summary>
	/// Create a copy of an object
	/// </summary>
	/// <param name="object">The object being copied</param>
	Object(const Object& object);

	inline uint64_t GetInstanceID();
	/// <summary>
	/// Returns the name of the object
	/// </summary>
	inline std::string ToString();

	/// <summary>
	/// Unregisters and frees the memory of this object
	/// </summary>
	void Destroy();
	/// <summary>
	/// Unregisters and frees the memory of an object
	/// </summary>
	/// <param name="obj">The object to delete</param>
	static void Destroy(Object* const obj);

	inline static Object* Instantiate(Object obj);

	/// <summary>
	/// Return whether the object exists
	/// </summary>
	inline virtual operator bool();
	/// <summary>
	/// Compares if two objects refer to a different object
	/// </summary>
	inline virtual int operator !=(Object rh);
	/// <summary>
	/// Comapres two object references to see if they refer to the same object
	/// </summary>
	inline virtual int operator ==(Object rh);

protected:

	/// <summary>
	/// Private delete that should only be called by ObjectManager
	/// </summary>
	~Object();

private:

	/// <summary>
	/// The unique ID of the object
	/// </summary>
	uint64_t ID;
};