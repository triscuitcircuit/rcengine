//
// LuaScript - per-entity Lua script runner using Sol2
//
#pragma once

#include "sol/sol.hpp"
#include <string>

// Forward declare to avoid circular dependency:
//   Component.h  -> LuaScript.h  (for LuaScriptComponent)
//   LuaScript.h  -> Entity.h     (would pull Component.h back in)
namespace RcEngine { class Entity; }

namespace LUtil {

    class LuaScript {
    public:
        LuaScript() = default;
        ~LuaScript() = default;

        // Called from Scene::OnRuntimeStart  to bind the entity and load the file.
        void Initialize(RcEngine::Entity& entity, const std::string& filepath);

        void OnCreate();
        void OnUpdate(float ts);
        void OnDestroy();

        bool IsValid() const { return m_Valid; }

    private:
        void RegisterBindings(RcEngine::Entity& entity);

        sol::state   m_State;
        sol::function m_OnCreate;
        sol::function m_OnUpdate;
        sol::function m_OnDestroy;
        bool m_Valid = false;
    };

}
