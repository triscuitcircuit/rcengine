//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once

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

#ifdef RC_ENABLE_ASSERTS
    #ifdef RC_PLATFORM_WINDOWS
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
