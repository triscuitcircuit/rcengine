//
// Created by Tristan Zippert on 5/28/21.
//

#pragma once

#include "rcpch.h"
#include <glm/glm.hpp>

#include <../external/GLFW/include/GLFW/glfw3.h>

namespace RcEngine{
    class FileDialog{
    public:
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);
    };
    class OpenGLUtils{
    public:
        OpenGLUtils();
        static void displayComputerShaderLimits();
        static bool CheckOpenGLError();

        static void checkOpenGLErrors();

        static void PrintShaderLog(unsigned int shader);
        static void printProgramLog(int prog);

        static unsigned int loadTexture(const char *texImagePath);
        static unsigned int loadCubeMap(const char *mapDir);

        static std::string readShaderFile(const char* filepath);

    };
}
