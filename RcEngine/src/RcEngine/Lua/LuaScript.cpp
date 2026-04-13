//
// Created by Tristan Zippert on 12/31/21.
//

#include "LuaScript.h"
#include "RcEngine/Scene/Component.h"
#include "luautil.h"

namespace LUtil{

    LuaScript::LuaScript(const char *filename) {
        try {
            LuaUtil::LuaUtil *lua_instance = new LuaUtil::LuaUtil();
            lua_instance->addScript(reinterpret_cast<std::string &>(filename));
        }
        catch (const sol::error& e) {
            RC_ASSERT(false, "Lua script error: %s", e.what());
        }
    }



}