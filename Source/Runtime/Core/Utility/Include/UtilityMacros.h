#pragma once

#include <cassert>

#ifdef _DEBUG
#define ASSERT(x) if (!(x)) { assert(false); };

#else 
#define ASSERT(x)

#endif // _DEBUG

#define	TRIVIAL_FUNCTION(x) \
	public:\
		x(void) = default; \
		x(const x&) = default; \
		x(x&&) = default; \
		x& operator=(const x&) = default; \
		x& operator=(x&&) = default; \
		~x(void) = default;

#define CONSTEXPR_TRIVIAL_FUNCTION(x)\
	public:\
		constexpr x(void) = default; \
		constexpr x(const x&) = default; \
		constexpr x(x&&) = default; \
		constexpr x& operator=(const x&) = default; \
		constexpr x& operator=(x&&) = default; \
		~x(void) = default;

//TODO Remove : i dont use xcept
#define TRIVIAL_FUNCTION_NOEXCEPT(x) \
	 x() noexcept = default; \
	 x(const x&) noexcept = default; \
	 x(x&&) noexcept = default; \
	 x& operator=(const x&) noexcept = default; \
	 x& operator=(x&&) noexcept = default; \
	 ~x(void) noexcept = default;


#define TEST_ERR(x) \
	if (x != ErrorCode::Success) { \
		 { assert(false); }\
	} \


#define HAS_FUNCTION(Class, Func) \
static_assert(requires(Class& obj) { obj.Func; }, \
"Class " #Class " missing function: " #Func)

#define HAS_NESTED_TYPE(Class, TypeName) \
static_assert(requires { typename Class::TypeName; }, \
"Class " #Class " missing nested type: " #TypeName)

#define HAS_MEMBER_VARIABLE(Class, Member) \
    static_assert(requires(Class& obj) { obj.Member; }, \
                  "Class " #Class " missing member variable: " #Member)