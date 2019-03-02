#include "ObjectManager.h"
#include "Component.h"
#include "GameObject.h"

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
	LOG_TRACE("Adding to object manager\t\t'{}'", object->name, object->ID);

	activeObjects.insert({ objectCount, object });
	objectCount++;
}


void ObjectManager::UnregisterObject(Object* const object)
{
	if (object->ID >= objectCount)
	{
		LOG_WARNING("Can't unregister object that isn't registered\t\t ID: {}", object->ID);
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
		LOG_WARNING("Can't unregister object that isn't registered\t\t ID: {}", objectID);
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
		LOG_WARNING("Can't delete object that isn't registered\t\t ID: {}", objectID);
		return;
	}

	Object* object = activeObjects[objectID];

	if (dynamic_cast<Component*>(object))
	{
		Component* component = dynamic_cast<Component*>(object);
		if (!component->gameObject->RemoveComponent(component))
		{
			return;
		}
	}

	LOG_TRACE("Destroying: {}", object->name);
	UnregisterObject(object);
	delete object;
}


void ObjectManager::DestroyObject(Object* object)
{
	if (object->ID >= objectCount)
	{
		LOG_WARNING("Can't delete object that isn't registered\t\t ID: {}", object->ID);
		return;
	}

	if (dynamic_cast<Component*>(object))
	{
		Component* component = dynamic_cast<Component*>(object);
		component->gameObject->RemoveComponent(component);
		return;
	}

	LOG_TRACE("Destroying: {}", object->name);
	UnregisterObject(object);
	delete object;
}
