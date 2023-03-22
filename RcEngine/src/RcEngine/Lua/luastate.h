//
// Created by Tristan Zippert on 12/30/21.
//

#ifndef RCENGINE_LUASTATE_H
#define RCENGINE_LUASTATE_H

#include "sol/sol.hpp"
#include <limits>
#include <map>


#define SOL_SAFE_USERTYPE 1
#define SOL_SAFE_REFERENCES 1
#define SOL_SAFE_FUNCTION_CALLS 1
#define SOL_SAFE_FUNCTION 1
#define SOL_NO_NIL 0
#define SOL_IN_DEBUG_DETECTED 0

#define SOL_LUAJIT 1
#define SOL_EXCEPTIONS_SAFE_PROPAGATION 0


namespace LUtil{
//    std::string getLuaVersion();

    class LuaState{
    public:

    LuaState(std::string& filename);
    ~LuaState();

    void initLua();

    };
}

#endif //RCENGINE_LUASTATE_H
