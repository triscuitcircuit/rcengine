//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once
#include <memory>

#ifdef _WIN32
    #ifdef _WIN64
        #define RC_PLATFORM_WINDOWS
    #else
        #error "x32-86 bit builds not supported"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS/IPHONE not supported"
    #elif TARGET_OS_IPHONE
        #define RC_PLATFORM_IOS
        #error "IOS not supported"
    #elif  TARGET_OS_MAC
        #define RC_PLATFORM_MAC
    #else
        #error "Unknown APPLE"
    #endif
#endif

#ifdef RC_PLATFORM_WINDOWS
    #ifdef RC_BUILD_DLL
        #define RC_API __declspec(dllexport)
    #else
        #define RC_API __declspec(dllimport)
    #endif
#else
    #ifdef RC_BUILD_DLL
        #define RC_API __attribute__ ((visibility ("default")))
       // #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        #define RC_API
      //  #define DLL_LOCAL
    #endif
#endif

#ifdef RC_DEBUG
#define RC_ENABLE_ASSERTS
#endif

#ifdef RC_ENABLE_ASSERTS
    #if defined(RC_PLATFORM_WINDOW) || defined(RC_PLATFORM_UNIX) ||defined(RC_PLATFORM_MAC)
        #define RC_ASSERT(x, ...) {if(!(x)){RC_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); }}
        #define RC_CORE_ASSERT(x, ...) {if(!(x)) {RC_CORE_ERROR("Assertion failed: {0}",__VA_ARGS__); __debugbreak();} }
    #else
        #define RC_ASSERT(x, ...) {if(!(x)){RC_ERROR("Assertion Failed: {0}",__VA_ARGS__); raise(SIGTRAP); }}
        #define RC_CORE_ASSERT(x, ...) {if(!(x)) {RC_CORE_ERROR("Assertion failed: {0}",__VA_ARGS__); raise(SIGTRAP);} }
    #endif
#else
    #define RC_ASSERT(x, ...)
    #define RC_CORE_ASSERT(x, ...)
#endif

#define RC_BIND_EVENT_TO_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

namespace RcEngine{
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}