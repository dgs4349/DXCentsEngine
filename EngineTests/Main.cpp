
#include <stdlib.h>
#include <iostream>

#include "Logger.h"
#include "ObjectManager.h"

int main(void)
{
	Logger* logger = Logger::GetInstance();

	ObjectManager* objMan = ObjectManager::GetInstance();

	getchar();
	return 1;
}