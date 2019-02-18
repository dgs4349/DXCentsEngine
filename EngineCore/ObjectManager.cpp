#include "ObjectManager.h"

using namespace std;

ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
	std::unordered_map<uint64_t, Object*>::iterator it = activeObjects.begin();

	for (it = activeObjects.begin(); it != activeObjects.end(); ++it)
	{
		delete (*it).second;
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


void ObjectManager::DestroyObject(Object* const object)
{
	if (object->ID >= objectCount)
	{
		LOG_WARNING("Tried to delete entity with ID:{} that doesn't exist", object->ID);
		return;
	}

	activeObjects.erase(activeObjects.find(object->ID));
	delete object;
}


void ObjectManager::DestroyObject(uint64_t objectID)
{
	if (objectID >= objectCount)
	{
		LOG_WARNING("Tried to delete object with ID:{} that doesn't exist", objectID);
		return;
	}

	activeObjects.erase(activeObjects.find(objectID));
	delete activeObjects[objectID];
}
