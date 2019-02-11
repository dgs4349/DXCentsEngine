#pragma once
#include <map>
#include <stdint.h>

#include "Object.h"
#include "Singleton.h"

class ObjectManager : public Singleton<ObjectManager>
{
	friend class Singleton<ObjectManager>;
	friend class Object;
public:

protected:

	void RegisterObject(Object* const object);
	void DeleteObject(Object* const object);
	void DeleteObject(uint64_t objectID);

	ObjectManager();
	~ObjectManager();

private:
	uint64_t objectCount = 0;

	std::unordered_map<uint64_t, Object*> activeObjects;
};
