
#include <iostream>

#include "CentsEngine.h"

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>


int main()
{
	ObjectManager::GetInstance();

	GameObject* obj = new GameObject("Test");

	Destroy(obj->transform);
	obj->AddComponent<Transform>();

	getchar();
	ObjectManager::ReleaseInstance();
	getchar();
	return 1;
}