export module Launch;

export import :Universal;
export import :Windows;

import Platform;
import Utility;

import ApplicationContext;

ErrorCode Launch(void){
	//Noting TODO :becase Windows is the only platform

	g_APP->StartUp();

	return ErrorCode::Success;
}
