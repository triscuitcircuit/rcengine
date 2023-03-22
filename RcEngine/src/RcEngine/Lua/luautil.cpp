//
// Created by Tristan Zippert on 8/17/22.
//

#include "luautil.h"
#include "RcEngine/Scene/Component.h"
#include "RcEngine/Scene/Entity.h"
#include "RcEngine/Core/Core.h"

static const std::string Packages[] = {"coroutine", "math", "string", "table"};

namespace LuaUtil{
    LuaUtil::LuaUtil() {
        m_Lua.open_libraries(sol::lib::base,sol::lib::coroutine,sol::lib::math, sol::lib::bit32,
                             sol::lib::string, sol::lib::table, sol::lib::os, sol::lib::debug);
        for(const std::string & s: Packages){
            RC_CORE_ASSERT(m_Lua[s] != sol::nil, "Lua function not found: %s" , s);
            m_Packages[s] = m_Lua[s];
        }
    }
    template<typename T>
    void LuaUtil::addVecMethods(sol::usertype<T>& VecType){
        VecType[sol::meta_function::equal_to] = [](const T& a, const T& b){return a==b;};
        VecType[sol::meta_function::addition] = [](const T& a, const T& b){return a+b;};
        VecType[sol::meta_function::subtraction] = [](const T& a, const T& b){return a-b;};
        VecType[sol::meta_function::multiplication] = sol::overload(
                [](const T& a, float c){return a * c;},
                [](const T& a, const T& b){return a * b;}
                );
        VecType[sol::meta_function::division] = [](const T& a, const T& b){return a/b;};

    }
    sol::table LuaUtil::initBasePackage(sol::state& luastate){

        sol::table util(luastate, sol::create);

        util["quit"] = [](){
            RC_INFO("Quit requested by Lua");
        };

        util["vector2"] = [](float x ,float y){return Vec2(x,y);};
        sol::usertype<Vec2> vectype2 = luastate.new_usertype<Vec2>("Vec2");
        vectype2["x"] = sol::property([](const Vec2& v)-> float{return v.x;});
        vectype2["y"] = sol::property([](const Vec2& v)-> float{return v.y;});
        vectype2["length"] = sol::readonly_property([](const Vec2& v)-> float{return v.length();});
        addVecMethods<Vec2>(vectype2);

        util["vector3"] = [](float x ,float y, float z){return Vec3(x,y,z);};
        sol::usertype<Vec3> vectype3 = luastate.new_usertype<Vec3>("Vec3");
        vectype3["x"] = sol::property([](const Vec3& v)-> float{return v.x;});
        vectype3["y"] = sol::property([](const Vec3& v)-> float{return v.y;});
        vectype3["z"] = sol::property([](const Vec3& v)-> float{return v.z;});
        vectype3["length"] = sol::readonly_property([](const Vec3& v)-> float{return v.length();});
        addVecMethods<Vec3>(vectype3);

        util["vector4"] = [](float x ,float y, float z, float w){return Vec4(x,y,z,w);};
        sol::usertype<Vec4> vectype4 = luastate.new_usertype<Vec4>("Vec4");
        vectype4["x"] = sol::property([](const Vec4& v)-> float{return v.x;});
        vectype4["y"] = sol::property([](const Vec4& v)-> float{return v.y;});
        vectype4["z"] = sol::property([](const Vec4& v)-> float{return v.z;});
        vectype4["w"] = sol::property([](const Vec4& v)-> float{return v.w;});
        vectype4["length"] = sol::readonly_property([](const Vec4& v)-> float{return v.length();});
        addVecMethods<Vec4>(vectype4);


        return util;
    }
    void LuaUtil::addPackage(std::string packageName, sol::object package){
        m_Packages.emplace(std::move(packageName), std::move(package));
    }

//    sol::table coreLuaPackage(sol::state& lua ){
//        namespace RcEngine{
//
//        }
//    }
}