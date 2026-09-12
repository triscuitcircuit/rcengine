//
// Created by Tristan Zippert on 12/30/21.
//

#ifndef RCENGINE_LUASTATE_H
#define RCENGINE_LUASTATE_H
#include "RcEngine/Scene/Entity.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "sol/sol.hpp"
#include <limits>
#include <map>


#define SOL_SAFE_USERTYPE 1
#define SOL_SAFE_REFERENCES 1
#define SOL_SAFE_FUNCTION_CALLS 1
#define SOL_SAFE_FUNCTION 1
#define SOL_NO_NIL 0
#define SOL_IN_DEBUG_DETECTED 0

#define SOL_LUAJIT 0
#define SOL_EXCEPTIONS_SAFE_PROPAGATION 0


namespace LUtil {
    class LuaScript {
    public:
        LuaScript(const char* filename);
        ~LuaScript();

        void Initialize(RcEngine::Entity& entity, const std::string& filepath);
        void RegisterBindings(RcEngine::Entity& entity);

        void OnCreate();
        void OnUpdate(float ts);
        void OnDestroy();

    private:
        bool b_hasLuaScript;
        bool m_Valid = false;

        sol::state m_State;
        sol::protected_function m_OnCreate;
        sol::protected_function m_OnUpdate;
        sol::protected_function m_OnDestroy;
    };
}
#endif //RCENGINE_LUASTATE_H
