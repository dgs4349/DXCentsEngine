#pragma once
#include <unordered_map>
#include <stdint.h>

#include "Singleton.h"
#include "Object.h"

#ifndef Object
class Object;
#endif

/// <summary>
/// Class that manages all of the objects that are created
/// This handles deleting all allocated objects
/// </summary>
class ObjectManager : public Singleton<ObjectManager>
{
	friend class Singleton<ObjectManager>;
	friend Object;

public:

	/// <summary>
	/// Unregister an object from the manager
	/// </summary>
	/// <param name="object">The object to unregister</param>
	void DestroyObject(Object* const object);


protected:

	/// <summary>
	/// Register an object to the manager
	/// </summary>
	/// <param name="object">The object to register</param>
	void RegisterObject(Object* const object);
	/// <summary>
	/// Unregister an object from the manager
	/// </summary>
	/// <param name="objectID">The unique object ID to unregister</param>
	void DestroyObject(uint64_t objectID);


protected:

	/// <summary>
	/// The amount of objects that the manager handles
	/// </summary>
	uint64_t objectCount = 0;

	ObjectManager();
	~ObjectManager();

private:

	/// <summary>
	/// All of the objects that have been registered
	/// </summary>
	std::unordered_map<uint64_t, Object*> activeObjects;
};

#define Destroy ObjectManager::GetInstance()->DestroyObject
