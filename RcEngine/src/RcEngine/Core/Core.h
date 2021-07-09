//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once
#include <memory>

#ifdef _MSC_VER
    #define DEBUG_BREAK __debugbreak()
#elif defined( __clang__ )
    #if __has_builtin(__builtin_debugtrap)
    #define DEBUG_BREAK __builtin_debugtrap()
#else
    #define DEBUG_BREAK __builtin_trap()
#endif
    #elif defined( __GNUC__ )
        #define DEBUG_BREAK __builtin_trap()
    #else
        #error "OS not recognized"
        #endif

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
     #define RC_ASSERT(x, ...) {if(!(x)){RC_ERROR("Assertion Failed: {0}",__VA_ARGS__); DEBUG_BREAK; }}
     #define RC_CORE_ASSERT(x, ...) {if(!(x)) {RC_CORE_ERROR("Assertion failed: {0}",__VA_ARGS__); DEBUG_BREAK;} }
#else
    #define RC_ASSERT(x, ...)
    #define RC_CORE_ASSERT(x, ...)
#endif

#define RC_BIND_EVENT_TO_FUNCTION(fn) [this](auto&&... args) -> decltype(auto){return this->fn(std::forward<decltype(args)>(args)...);}

namespace RcEngine{
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ...args){
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
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