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
			instance = new T();
		}
		return instance;
	}

	static void ReleaseInstance(void)
	{
		delete GetInstance();
	}

	Singleton(Singleton const&) = delete;
	void operator=(Singleton const&) = delete;

protected:
	Singleton()
	{
		LOG_TRACE("Created '{}' Singleton", typeid(T).name());
	}
	~Singleton()
	{
		LOG_TRACE("Released '{}' Singleton", typeid(T).name());
	}
};