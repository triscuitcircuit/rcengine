//
// LuaScript.cpp - per-entity Lua script implementation
//
#include "rcpch.h"
#include "LuaScript.h"

// Full types needed here (not in header, to avoid circular dependencies)
#include "RcEngine/Scene/Entity.h"
#include "RcEngine/Scene/Component.h"
#include "RcEngine/Core/Input.h"
#include "RcEngine/Core/keycodes.h"

namespace LUtil {

    // ---------------------------------------------------------------- //
    //  RegisterBindings – expose engine types to this entity's Lua state //
    // ---------------------------------------------------------------- //
    void LuaScript::RegisterBindings(RcEngine::Entity& entity) {
        m_State.open_libraries(
            sol::lib::base,
            sol::lib::math,
            sol::lib::string,
            sol::lib::table,
            sol::lib::os
        );

        // ---- glm::vec3 ----
        m_State.new_usertype<glm::vec3>("Vec3",
            sol::constructors<glm::vec3(), glm::vec3(float,float,float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z,
            sol::meta_function::addition,
                [](const glm::vec3& a, const glm::vec3& b){ return a + b; },
            sol::meta_function::subtraction,
                [](const glm::vec3& a, const glm::vec3& b){ return a - b; },
            sol::meta_function::multiplication,
                sol::overload(
                    [](const glm::vec3& a, float s){ return a * s; },
                    [](const glm::vec3& a, const glm::vec3& b){ return a * b; }
                )
        );

        // ---- TransformComponent ----
        m_State.new_usertype<RcEngine::TransformComponent>("TransformComponent",
            "Translation", &RcEngine::TransformComponent::Translation,
            "Rotation",    &RcEngine::TransformComponent::Rotation,
            "Scale",       &RcEngine::TransformComponent::Scale
        );

        // ---- GetTransform() captures entity by value (safe: Entity is a lightweight handle) ----
        m_State["GetTransform"] = [entity]() mutable -> RcEngine::TransformComponent& {
            return entity.GetComponent<RcEngine::TransformComponent>();
        };

        m_State["HasTransform"] = [entity]() mutable -> bool {
            return entity.HasComponent<RcEngine::TransformComponent>();
        };

        // ---- Input ----
        m_State["IsKeyPressed"] = [](int key) -> bool {
            return RcEngine::Input::IsKeyPressed(static_cast<RcEngine::KeyCode>(key));
        };

        auto keys = m_State.create_table();
        keys["W"]           = (int)RcEngine::Key::W;
        keys["A"]           = (int)RcEngine::Key::A;
        keys["S"]           = (int)RcEngine::Key::S;
        keys["D"]           = (int)RcEngine::Key::D;
        keys["Space"]       = (int)RcEngine::Key::Space;
        keys["LeftShift"]   = (int)RcEngine::Key::LeftShift;
        keys["LeftControl"] = (int)RcEngine::Key::LeftControl;
        keys["Up"]          = (int)RcEngine::Key::Up;
        keys["Down"]        = (int)RcEngine::Key::Down;
        keys["Left"]        = (int)RcEngine::Key::Left;
        keys["Right"]       = (int)RcEngine::Key::Right;
        m_State["Key"]      = keys;

        // ---- Logging ----
        auto log = m_State.create_table();
        log["Info"]  = [](const std::string& msg){ RC_INFO(msg); };
        log["Warn"]  = [](const std::string& msg){ RC_WARN(msg); };
        log["Error"] = [](const std::string& msg){ RC_ERROR(msg); };
        m_State["Log"] = log;
    }

    // ---------------------------------------------------------------- //
    //  Initialize                                                        //
    // ---------------------------------------------------------------- //
    void LuaScript::Initialize(RcEngine::Entity& entity, const std::string& filepath) {
        RegisterBindings(entity);

        auto result = m_State.load_file(filepath);
        if (!result.valid()) {
            sol::error e = result;
            RC_CORE_ERROR("Lua load error in '{0}': {1}", filepath, e.what());
            m_Valid = false;
            return;
        }

        // Execute the script to define all functions
        auto execResult = result();
        if (!execResult.valid()) {
            sol::error e = execResult;
            RC_CORE_ERROR("Lua exec error in '{0}': {1}", filepath, e.what());
            m_Valid = false;
            return;
        }

        m_OnCreate  = m_State["OnCreate"];
        m_OnUpdate  = m_State["OnUpdate"];
        m_OnDestroy = m_State["OnDestroy"];
        m_Valid = true;
    }

    // ---------------------------------------------------------------- //
    //  Lifecycle callbacks                                               //
    // ---------------------------------------------------------------- //
    void LuaScript::OnCreate() {
        if (!m_Valid || !m_OnCreate.valid()) return;
        auto res = m_OnCreate();
        if (!res.valid()) {
            sol::error e = res;
            RC_CORE_ERROR("Lua OnCreate error: {0}", e.what());
        }
    }

    void LuaScript::OnUpdate(float ts) {
        if (!m_Valid || !m_OnUpdate.valid()) return;
        auto res = m_OnUpdate(ts);
        if (!res.valid()) {
            sol::error e = res;
            RC_CORE_ERROR("Lua OnUpdate error: {0}", e.what());
        }
    }

    void LuaScript::OnDestroy() {
        if (!m_Valid || !m_OnDestroy.valid()) return;
        auto res = m_OnDestroy();
        if (!res.valid()) {
            sol::error e = res;
            RC_CORE_ERROR("Lua OnDestroy error: {0}", e.what());
        }
    }

}
