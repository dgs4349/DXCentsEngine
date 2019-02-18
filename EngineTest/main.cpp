#include "CentsEngine.h"

#include <iostream>

int main(void)
{
	ObjectManager::GetInstance();
	
	GameObject* obj = new GameObject(); 

	obj->AddComponent<Object>();
	obj->AddComponent<Transform>();
	obj->AddComponent<Transform>();

	getchar();
	ObjectManager::ReleaseInstance();
	getchar();
	return 0;
}