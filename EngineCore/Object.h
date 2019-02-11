#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "ObjectManager.h"

class Object
{
	friend class ObjectManager;
public:

	std::string name = "Object";

	Object();
	Object(const Object& object);

	inline uint64_t GetInstanceID();
	inline std::string ToString();

	static void Destroy(Object* const obj);

	inline static Object* Instantiate(Object obj);

	inline virtual operator bool();
	inline virtual int operator !=(Object rh);
	inline virtual int operator ==(Object rh);

protected:

	~Object();

private:

	uint64_t ID;
};