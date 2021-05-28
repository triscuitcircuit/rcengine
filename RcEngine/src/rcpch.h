//
// Created by Tristan Zippert on 5/20/21.
//
#pragma once

// Included headers for RcEngine applications

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

#include <string>
#include <vector>
#include <sstream>

#include "RcEngine/Log.h"
#include "RcEngine/Core/Core.h"

///Renderer////////////////////////////
#include "RcEngine/Renderer/Renderer.h"
#include "RcEngine/Renderer/Buffer.h"
#include "RcEngine/Renderer/Shader.h"

#ifdef RC_PLATFORM_WINDOWS
    #include <Windows.h>
#endif
