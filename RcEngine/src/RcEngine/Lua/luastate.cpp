//
// Created by Tristan Zippert on 12/31/21.
//

#include "luastate.h"

namespace LUtil{
    std::string getLuaVersion(){
        return LUA_RELEASE;
    }

    void LuaState::addPackage(std::string packageName, sol::object package){
        m_Packages.emplace(std::move(packageName), std::move(package));
    }
    sol::function LuaState::loadScript(const std::string &path) {
        auto iter = m_Compiled.find(path);
        if(iter != m_Compiled.end())
            return m_Lua.load(iter->second.as_string_view(),path,sol::load_mode::binary);
        FILE *f;
        if((f = fopen(path.c_str(),"rb") )== NULL){
            RC_ASSERT(0,"Error finding lua file");
        };
        //std::string fileContent(f,{});
        //sol::load_result res = m_Lua.load(fileContent,path,sol::load_mode::text);

    }
}