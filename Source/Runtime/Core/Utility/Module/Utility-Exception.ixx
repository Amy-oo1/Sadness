export module Utility:Exception;

import Platform;

//NOTE : Error Codes
export enum class ErrorCode : Uint32 {
	Success = 0,
	Failure = 1,
	InvalidArgument = 2,
	NotImplemented = 3,
	OutOfMemory = 4,
	InvalidState = 5,
	InvalidOperation = 6,
	InvalidHandle = 7,
	InvalidPointer = 8,
	InvalidSize = 9,
	InvalidType = 10,
};