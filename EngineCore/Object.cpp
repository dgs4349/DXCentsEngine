#include "Object.h"

#include "Logger.h"

Object::Object()
{
	ObjectManager::GetInstance()->RegisterObject(this);
}


Object::Object(const Object& object)
{
	name = object.name;
	ObjectManager::GetInstance()->RegisterObject(this);
}


Object::Object(std::string name)
{
	this->name = name;
	ObjectManager::GetInstance()->RegisterObject(this);
}


Object::~Object()
{
	LOG_TRACE("Destroying {}:{}", name, ID);
	ObjectManager::GetInstance()->UnregisterObject(this);
}


uint64_t Object::GetInstanceID()
{
	return ID;
}


std::string Object::ToString()
{
	return name;
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