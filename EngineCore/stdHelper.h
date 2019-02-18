#pragma once

#include <string>

template <class T>
std::string GetTypeName()
{
	std::string name = typeid(T).name();
	name = name.substr(name.find(' ') + 1, name.size());

	return name;
}