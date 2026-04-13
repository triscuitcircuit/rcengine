//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once
#include <filesystem>

#ifdef RC_ENABLE_ASSERT
    #define RC_ASSERT(...)
    #define RC_CORE_ASSERT(...)
#else
    #define RC_ASSERT(...)
    #define RC_CORE_ASSERT(...)
#endif