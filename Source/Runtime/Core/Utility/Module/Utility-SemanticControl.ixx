export module Utility : SemanticControl;

//NOTE : this class used by base class,su class can not use this class
// if you device a class and use this class ,you's class will has tow base class

//NOTE : Non-Restraint
export class NonDefaultConstructible {
private:
	NonDefaultConstructible(void) = delete;

protected:
	NonDefaultConstructible(const NonDefaultConstructible&) = default;
	NonDefaultConstructible& operator=(const NonDefaultConstructible&) = default;

	NonDefaultConstructible(NonDefaultConstructible&&) = default;
	NonDefaultConstructible& operator=(NonDefaultConstructible&&) = default;
};

export class CopyAbleOnly {
private:
	CopyAbleOnly(CopyAbleOnly&&) = delete;
	CopyAbleOnly& operator=(CopyAbleOnly&&) = delete;

protected:
	CopyAbleOnly(void) = default;

	CopyAbleOnly(const CopyAbleOnly&) = default;
	CopyAbleOnly& operator=(const CopyAbleOnly&) = default;
};

export class MoveAbleOnly {
private:
	MoveAbleOnly(const MoveAbleOnly&) = delete;
	MoveAbleOnly& operator=(const MoveAbleOnly&) = delete;
protected:
	MoveAbleOnly(void) = default;

	MoveAbleOnly(MoveAbleOnly&&) = default;
	MoveAbleOnly& operator=(MoveAbleOnly&&) = default;
};

export class NonCpyeAndMoveAble {
private:
	NonCpyeAndMoveAble(const NonCpyeAndMoveAble&) = delete;
	NonCpyeAndMoveAble& operator=(const NonCpyeAndMoveAble&) = delete;

	NonCpyeAndMoveAble(NonCpyeAndMoveAble&&) = delete;
	NonCpyeAndMoveAble& operator=(NonCpyeAndMoveAble&&) = delete;
protected:
	NonCpyeAndMoveAble(void) = default;
};

export class Singleton {
private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
protected:
	Singleton(void) = default;
};

//NOTE : Must-Restraint
//Because virtual function ,i dont not wangt to use,so this part TODO