module;

#include "ThirdParty/spdlog/Include/spdlog/spdlog.h"
#include "ThirdParty/spdlog/Include/spdlog/async.h"
#include "ThirdParty/spdlog/Include/spdlog/sinks/stdout_color_sinks.h"

#include "Runtime/Core/Platform/Include/PlatformMacros.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Logger;

import Platform;
import Utility;
import Container;

using namespace spdlog::level;

export class SystemLogger final :public Singleton {
public:
	enum class Level : Uint32 {
		
		Trace = static_cast<Uint32>(trace),
		Debug = static_cast<Uint32>(debug),
		Info = static_cast<Uint32>(info),
		Warn = static_cast<Uint32>(warn),
		Err = static_cast<Uint32>(err),
		Critical = static_cast<Uint32>(critical)
	};

private:
	SystemLogger(void);

public:
	~SystemLogger(void);

public:
	static SystemLogger& Get(void) {
		static SystemLogger instance{};

		return instance;
	}

	template<typename... TARGS>
	void Log(Level Level, TARGS&&... Args)const {
		switch (Level) {
			using enum SystemLogger::Level;
		case Trace:
			this->m_Logger->trace(std::forward<TARGS>(Args)...);
			break;
		case Debug:
			this->m_Logger->debug(std::forward<TARGS>(Args)...);
			break;
		case Info:
			this->m_Logger->info(std::forward<TARGS>(Args)...);
			break;
		case Warn:
			this->m_Logger->warn(std::forward<TARGS>(Args)...);
			break;
		case Err:
			this->m_Logger->error(std::forward<TARGS>(Args)...);
			break;
		case Critical:
			this->m_Logger->critical(std::forward<TARGS>(Args)...);
			break;
		default:
			ASSERT(false);
			break;
		}
	}

	SharedPtr<spdlog::logger> m_Logger{ nullptr };
};

SystemLogger::SystemLogger(void){
	//Consoule Sink
	auto ConsoleSink{ std::make_shared<spdlog::sinks::stdout_color_sink_mt>() };
	ConsoleSink->set_level(trace);
	ConsoleSink->set_pattern("[%H:%M:%S %e] [%^%l%$] [thread %t] %v");

	//Default Get_Instance Thread Pool
	spdlog::init_thread_pool(8192, 1);

	this->m_Logger = MakeShared<spdlog::async_logger>("System_Logger", ConsoleSink, spdlog::thread_pool(), spdlog::async_overflow_policy::block);
	this->m_Logger->set_level(trace);
	spdlog::register_logger(this->m_Logger);
}

SystemLogger::~SystemLogger(void){
	this->m_Logger->flush();
	spdlog::drop_all();
}
