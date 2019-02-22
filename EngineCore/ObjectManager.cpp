#include "ObjectManager.h"

using namespace std;

ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
	std::unordered_map<uint64_t, Object*>::iterator it = activeObjects.begin();

	while ((it = activeObjects.begin()) != activeObjects.end())
	{
		if (it->second != NULL)
		{
			delete it->second;
		}
		else
		{
			UnregisterObject(it->first);
		}
	}

	activeObjects.clear();
}


void ObjectManager::RegisterObject(Object* const object)
{
	object->ID = objectCount;
	LOG_TRACE("Adding '{}:{}' to object manager", object->name, object->ID);

	activeObjects.insert({ objectCount, object });
	objectCount++;
}


void ObjectManager::UnregisterObject(Object* const object)
{
	if (object->ID >= objectCount)
	{
		LOG_WARNING("Tried to unregister object with ID:{} that isn't registered", object->ID);
		return;
	}

	if (activeObjects.find(object->ID) == activeObjects.end())
	{
		return;
	}

	activeObjects.erase(activeObjects.find(object->ID));
}

void ObjectManager::UnregisterObject(uint64_t objectID)
{
	if (objectID >= objectCount)
	{
		LOG_WARNING("Tried to unregister object with ID:{} that isn't registered", objectID);
		return;
	}

	if (activeObjects.find(objectID) == activeObjects.end())
	{
		return;
	}

	activeObjects.erase(activeObjects.find(objectID));
}


void ObjectManager::DestroyObject(uint64_t objectID)
{
	if (objectID >= objectCount)
	{
		LOG_WARNING("Tried to delete object with ID:{} that isn't registered", objectID);
		return;
	}

	Object* object = activeObjects[objectID];

	LOG_TRACE("Destroying: {}", object->name);
	UnregisterObject(object);
	delete object;
}


void ObjectManager::DestroyObject(Object* object)
{
	if (object->ID >= objectCount)
	{
		LOG_WARNING("Tried to delete object with ID:{} that isn't registered", object->ID);
		return;
	}

	LOG_TRACE("Destroying: {}", object->name);
	UnregisterObject(object);
	delete object;
}
