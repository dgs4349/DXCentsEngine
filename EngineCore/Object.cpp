#include "Object.h"



Object::Object()
{
}


Object::~Object()
{
}

uint64_t Object::GetInstanceID()
{
	return uint64_t();
}

#pragma region Operators

Object::operator bool()
{
}

int Object::operator!=(Object rh)
{
	return 0;
}

int Object::operator==(Object rh)
{
	return false;
}

#pragma endregion