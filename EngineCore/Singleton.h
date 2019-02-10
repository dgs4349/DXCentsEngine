#pragma once

#include "Logger.h"

template <class T>
class Singleton
{
public:

	static T* GetInstance(void)
	{
		static T* instance = nullptr;
		if (instance == nullptr)
		{
			LOG_TRACE( "Creating Singleton for '{}'", typeid(T).name());
			instance = new T();
		}
		return instance;
	}

	static void ReleaseInstance(void)
	{
	}

	Singleton(Singleton const&) = delete;
	void operator=(Singleton const&) = delete;

protected:
	Singleton() {}
	~Singleton() {}
};