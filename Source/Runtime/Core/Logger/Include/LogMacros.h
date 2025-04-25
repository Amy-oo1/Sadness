#pragma once

#define LOG_TRACE(...) SystemLogger::Get().Log(SystemLogger::Level::Trace, __VA_ARGS__)
#define LOG_DEBUG(...) SystemLogger::Get().Log(SystemLogger::Level::Debug, __VA_ARGS__)
#define LOG_INFO(...) SystemLogger::Get().Log(SystemLogger::Level::Info, __VA_ARGS__)
#define LOG_WARN(...) SystemLogger::Get().Log(SystemLogger::Level::Warn, __VA_ARGS__)
#define LOG_ERROR(...) SystemLogger::Get().Log(SystemLogger::Level::Err, __VA_ARGS__)
#define LOG_CRITICAL(...) SystemLogger::Get().Log(SystemLogger::Level::Critical, __VA_ARGS__)
