#include "Object.h"

#include "Logger.h"

Object::Object()
{
	ObjectManager::GetInstance()->RegisterObject(this);
}

Object::Object(const Object& object)
{
	ObjectManager::GetInstance()->RegisterObject(this);
}

Object::~Object()
{
	LOG_TRACE("Destroying {}:{}", name, ID);
}

uint64_t Object::GetInstanceID()
{
	return ID;
}

inline std::string Object::ToString()
{
	return name;
}

void Object::Destroy(Object* const obj)
{
	ObjectManager::GetInstance()->DeleteObject(obj);
}

Object* Object::Instantiate(Object obj)
{
	return new Object(obj);
}

#pragma region Operators

Object::operator bool()
{
	return this != NULL;
}

int Object::operator!=(Object rh)
{
	return ID != rh.ID;
}

int Object::operator==(Object rh)
{
	return ID == rh.ID;
}

#pragma endregion