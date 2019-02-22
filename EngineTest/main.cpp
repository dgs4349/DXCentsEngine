
#include <iostream>

#include "CentsEngine.h"

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>


void Print(std::string message)
{
	LOG_TRACE("{}", message);
}

void Print2(std::string message)
{
	LOG_TRACE("{}", message);
}

int main()
{
	ObjectManager::GetInstance();

	GameObject* obj = new GameObject("Test");

	Destroy(obj->transform);

	getchar();
	ObjectManager::ReleaseInstance();
	getchar();
	return 1;
}