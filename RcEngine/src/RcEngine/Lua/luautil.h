//
// Created by Tristan Zippert on 8/17/22.
//

#ifndef RCGAMEENGINE_LUAUTIL_H
#define RCGAMEENGINE_LUAUTIL_H
#include <glm/glm.hpp>
#include <sol/sol.hpp>
namespace LuaUtil {
    class LuaUtil {
        LuaUtil();
        ~LuaUtil();

        using Vec2 = glm::vec2;
        using Vec3 = glm::vec3;
        using Vec4 = glm::vec4;

        sol::table initBasePackage(sol::state &);

        template<typename T>
        void addVecMethods(sol::usertype<T> &VecType);

        void addPackage(std::string packageName, sol::object package);

    private:
        std::map<std::string, sol::bytecode> m_Compiled;
        std::map<std::string, sol::object> m_Packages;
        sol::state m_Lua;
    };
}
#endif //RCGAMEENGINE_LUAUTIL_H
