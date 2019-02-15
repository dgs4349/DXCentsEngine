#pragma once
#include <map>
#include <stdint.h>

#include "Object.h"
#include "Singleton.h"

/// <summary>
/// Class that manages all of the objects that are created
/// This handles deleting all allocated objects
/// </summary>
class ObjectManager : public Singleton<ObjectManager>
{
	friend class Singleton<ObjectManager>;
	friend class Object;
public:

protected:

	/// <summary>
	/// Register an object to the manager
	/// </summary>
	/// <param name="object">The object to register</param>
	void RegisterObject(Object* const object);
	/// <summary>
	/// Unregister an object from the manager
	/// </summary>
	/// <param name="object">The object to unregister</param>
	void DeleteObject(Object* const object);
	/// <summary>
	/// Unregister an object from the manager
	/// </summary>
	/// <param name="objectID">The unique object ID to unregister</param>
	void DeleteObject(uint64_t objectID);

protected:
	ObjectManager();
	~ObjectManager();

private:

	/// <summary>
	/// The amount of objects that the manager handles
	/// </summary>
	uint64_t objectCount = 0;

	/// <summary>
	/// All of the objects that have been registered
	/// </summary>
	std::unordered_map<uint64_t, Object*> activeObjects;
};
