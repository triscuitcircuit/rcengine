//
// Created by Tristan Zippert on 5/20/21.
//
#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdio>


#include <external/glm/glm/glm.hpp>
#include <external/GLFW/include/GLFW/glfw3.h>
#include <external/imgui/imgui.h>

#include "RcEngine/Core/Application.h"
#include "RcEngine/EntryPoint.h"
#include "RcEngine/Log.h"
#include "RcEngine/Layer.h"

#include "RcEngine/Core/Timestep.h"

#include "RcEngine/MouseButtonCodes.h"
#include "RcEngine/keycodes.h"
#include "RcEngine/Input.h"

#include "RcEngine/ImGui/ImGuiLayer.h"

///Renderer////////////////////////////
#include "RcEngine/Renderer/Renderer.h"
#include "RcEngine/Renderer/RenderCommand.h"
#include "RcEngine/Renderer/RenderAPI.h"
#include "RcEngine/Renderer/Buffer.h"
#include "RcEngine/Renderer/Shader.h"
#include "RcEngine/Renderer/VertexArray.h"
#include "RcEngine/Renderer/GraphicsContext.h"
#include "RcEngine/Renderer/OrthoCamera.h"
///////////////////////////////////////