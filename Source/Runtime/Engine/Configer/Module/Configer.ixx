module;
#include "Runtime/Engine/Configer/Include/ConfigMacros.h"

export module Configer;

import Platform;
import Utility;
import String;

export class Configer final :public Singleton {
private:
	Configer(void) {
		//TODO :
	}

public:
	~Configer(void) = default;

	static Configer& Get(void) {
		static Configer instance{};

		return instance;
	}

	//TODO Remove Macros and constexpr ,fix confige in runtime 
	static const constexpr TChar* s_Separators{ SEPARATORS };
	/*static inline Uint32 s_DefaultWindowWidth= WINDOW_WIDTH;
	static inline Uint32 s_DefaultWindowHeight = WINDOW_HEIGHT;*/

	//NOTE :this Configer Configer Sone Field that i do not knoew how to put
	//So I Put it window widot and heigth in window ,if you want to config in begin ,you kan use commandline or #define to Make File
};