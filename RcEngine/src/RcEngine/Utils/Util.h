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
}
