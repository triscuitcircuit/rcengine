//
// Created by Tristan Zippert on 5/30/21.
//
#include "rcpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace RcEngine{
    RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI;
}