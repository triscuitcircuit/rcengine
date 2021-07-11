//
// Created by Tristan Zippert on 7/9/21.
//
#pragma once
#include <string>

#ifndef RCENGINE_PLATFORMUTILS_H
#define RCENGINE_PLATFORMUTILS_H
namespace RcEngine{
    class FileDialogs{
    public:
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);
    };

}
#endif //RCENGINE_PLATFORMUTILS_H
