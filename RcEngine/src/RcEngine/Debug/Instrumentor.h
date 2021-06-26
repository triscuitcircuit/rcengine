//
// Created by Tristan Zippert on 6/26/21.
//
#pragma once
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>
namespace RcEngine{
    struct ProfileResult{
        std::string name;
        long long Start, End;
        uint32_t ThreadID;
    };
    struct InstrumentationSession{
        std::string Name;
    };
    class Instrumentor{
    private:
        InstrumentationSession* m_currentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
    public:
        Instrumentor()
        : m_currentSession(nullptr), m_ProfileCount(0){

        }
        void BeginSession(const std::string& name, const std::string& filepath = "results.json"){
            m_OutputStream.open(filepath);
            WriteHeader();
            m_currentSession = new InstrumentationSession{name};
        }

        void EndSession(){
            WriteFooter();
            m_OutputStream.close();
            delete m_currentSession;
            m_currentSession = nullptr;
            m_ProfileCount = 0;
        }

        void WriteProfile(const ProfileResult& result) {
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            std::string name = result.name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start;
            m_OutputStream << "}";

            m_OutputStream.flush();
        }
        void WriteHeader(){
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            m_OutputStream.flush();
        }

        void WriteFooter(){
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        static Instrumentor& Get(){
            static Instrumentor instance;
            return instance;
        }
    };
    class InstrumentationTimer{
    public:
        InstrumentationTimer(const char* name)
        :m_Name(name), m_Stopped(false){
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }
        ~InstrumentationTimer(){
            if(!m_Stopped)
                Stop();
        }
        void Stop(){
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

            m_Stopped = true;
        }
    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}
#define RC_PROFILE 1

#if RC_PROFILE
    #define RC_PROFILE_BEGIN_SESSION(name, filepath) ::RcEngine::Instrumentor::Get().BeginSession(name, filepath)
    #define CAT(A,B) A ## B
    #define RC_PROFILE_SCOPE(name) ::RcEngine::InstrumentationTimer CAT(timer,__LINE__)(name);
    #define RC_PROFILE_END_SESSION() ::RcEngine::Instrumentor::Get().EndSession()
    #define RC_PROFILE_FUNCTION() RC_PROFILE_SCOPE(__FUNCTION__)
#else
    #define RC_PROFILE_BEGIN_SESSION(name, filepath)
    #define RC_PROFILE_SCOPE(name)
    #define RC_PROFILE_END_SESSION()
    #define RC_PROFILE_FUNCTION()
#endif
