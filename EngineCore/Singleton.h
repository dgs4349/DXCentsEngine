#pragma once

#include "Logger.h"

template <class T>
class Singleton
{
public:

	/// <summary>
	/// Get the instance of this class
	/// If an instance doesn't reference anything the class constructor is called
	/// </summary>
	/// <returns>A reference to the class instance</returns>
	static T* GetInstance(void)
	{
		static T* instance = nullptr;
		if (instance == nullptr)
		{
			instance = new T();
		}
		return instance;
	}

	/// <summary>
	/// Release the instance of the class
	/// </summary>
	static void ReleaseInstance(void)
	{
		delete GetInstance();
	}

	/// <summary>
	/// Prevent the copy constructor from being called
	/// </summary>
	Singleton(Singleton const&) = delete;
	/// <summary>
	/// Prevent the set operation from happening
	/// </summary>
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