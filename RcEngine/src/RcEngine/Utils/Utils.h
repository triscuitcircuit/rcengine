//
// Created by Tristan Zippert on 5/28/21.
//

#pragma once

#include <string>
#include <optional>

namespace RcEngine{
    class FileDialog{
    public:
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);
    };
    class Utils{
    public:
        Utils();
        static void displayComputerShaderLimits();
        static void checkOpenGLErrors();

        static void PrintShaderLog(GLuint shader);

        static bool CheckOpenGLError();
        static void printProgramLog(int prog);
    };
}
