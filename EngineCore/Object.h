#pragma once
#include <stdint.h>
#include <string>
#include <vector>

class Object
{
public:

	std::string name;

	Object();
	~Object();

	inline uint64_t GetInstanceID();

	static void Destroy(Object obj);
	static Object FindObjectOfType();
	static std::vector<Object> FindObjectsOfType();
	static Object Instantiate(Object obj);

	inline operator bool();
	inline int operator !=(Object rh);
	inline int operator ==(Object rh);
};

