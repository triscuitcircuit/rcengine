//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include "spdlog/fmt/ostr.h"
#include "glm/gtx/string_cast.hpp"

namespace RcEngine{
    class RC_API Log{
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger(){return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger(){return s_ClientLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetEditorConsoleLogger(){return s_EditorConsoleLogger;}
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
        static std::shared_ptr<spdlog::logger> s_EditorConsoleLogger;
    };
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator << (OStream& os, const glm::vec<L,T,Q>& vector){
    return os<< glm::to_string(vector);
}
template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator << (OStream& os, const glm::mat<C,R,T,Q>& matrix){
    return os<< glm::to_string(matrix);
}

#define RC_CORE_ERROR(...) ::RcEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define RC_CORE_WARN(...) ::RcEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RC_CORE_INFO(...) ::RcEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RC_CORE_TRACE(...) ::RcEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
//#define RC_CORE_FATAL(...) ::RcEngine::Log::GetCoreLogger()->error(__VA_ARGS__)

//Client log macros

#define RC_ERROR(...) ::RcEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define RC_WARN(...) ::RcEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RC_INFO(...) ::RcEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define RC_TRACE(...) ::RcEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
//#define RC_FATAL(...) ::RcEngine::Log::GetClientLogger()->(__VA_ARGS__)

//Editor Console logging macros
