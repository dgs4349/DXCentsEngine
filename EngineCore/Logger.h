#pragma once

#include <iostream>

#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON

#include "spdlog.h"
#include "sinks\stdout_sinks.h"

class Logger
{
public:

	/// <summary>
	/// Get the Singleton instance of the Logger
	/// </summary>
	static Logger* GetInstance(void);
	/// <summary>
	/// Release the only instance of the Logger
	/// </summary>
	static void ReleaseInstance(void);

	/// <summary>
	/// Get the console the Logger is outputting to
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<spdlog::logger> GetCurrentConsole();

	Logger(Logger const&) = delete;
	void operator=(Logger const&) = delete;

protected:

	Logger();
	~Logger();

private:

	/// <summary>
	/// Shared pointer to the output conole 
	/// </summary>
	std::shared_ptr<spdlog::logger> console;
};

#if defined( DEBUG ) || defined ( _DEBUG )

/// <summary>
/// Log a regular type message
/// </summary>
#define  LOG_TRACE( ... )    Logger::GetCurrentConsole()->info( __VA_ARGS__ )
/// <summary>
/// Log a warning type message
/// </summary>
#define  LOG_WARNING( ... )     Logger::GetCurrentConsole()->warn( __VA_ARGS__ )
/// <summary>
/// Log an error type message
/// </summary>
#define  LOG_ERROR( ... )    Logger::GetCurrentConsole()->error( __VA_ARGS__ )

#else

#define  LOG_TRACE( ... ) 
#define  LOG_WARNING( ... )  
#define  LOG_ERROR( ... ) 

#endif