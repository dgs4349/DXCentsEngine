#pragma once
#include "Singleton.h"

class ObjectManager : public Singleton<ObjectManager>
{
	friend class Singleton<ObjectManager>;
public:

protected:
	ObjectManager();
	~ObjectManager();
};
