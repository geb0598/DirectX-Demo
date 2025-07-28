#pragma once

#ifdef _DEBUG

#include <iostream>
#include <sstream>
#include <string>

#include <Windows.h>

template<typename... Args>
void LogMessage(const char* Level, const char* File, int Line, Args&&... args)
{
	std::ostringstream Formatter;
	Formatter << "[" << Level << "]" << File << "(" << Line << "): ";
	(Formatter << ... << std::forward<Args>(args));
	Formatter << "\n";

	OutputDebugStringA(Formatter.str().c_str());
}

#define LOG_FATAL(...) LogMessage("FATAL", __FILE__, __LINE__, __VA_ARGS__); DebugBreak();
#define LOG_ERROR(...) LogMessage("ERROR", __FILE__, __LINE__, __VA_ARGS__); 
#define LOG_WARN(...)  LogMessage("WARN", __FILE__, __LINE__, __VA_ARGS__);
#define LOG_INFO(...)  LogMessage("INFO", __FILE__, __LINE__, __VA_ARGS__);
#define LOG_DEBUG(...) LogMessage("DEBUG", __FILE__, __LINE__, __VA_ARGS__);
#define LOG_TRACE(...) LogMessage("TRACE", __FILE__, __LINE__, __VA_ARGS__);

#else

#define LOG_FATAL(...)
#define LOG_ERROR(...)
#define LOG_WARN(...)
#define LOG_INFO(...)
#define LOG_DEBUG(...)
#define LOG_TRACE(...)

#endif