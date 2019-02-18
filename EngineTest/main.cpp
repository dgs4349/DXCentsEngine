#include "CentsEngine.h"

#include <iostream>

int main(void)
{
	ObjectManager::GetInstance();
	
	GameObject* obj = new GameObject("Test Object");

	LOG_TRACE("{}, {}", obj->ToString(), obj->transform->ToString());

	getchar();
	ObjectManager::ReleaseInstance();
	getchar();
	return 0;
}