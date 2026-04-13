//
// Created by Tristan Zippert on 5/30/21.
//

#include "rcpch.h"
#include "RenderAPI.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace RcEngine{
    RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;
}