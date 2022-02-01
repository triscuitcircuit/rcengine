//
// Created by Tristan Zippert on 10/14/21.
//
#include "rcpch.h"
#include "UUID.h"

#include <random>
#include <unordered_map>

static std::unordered_map<RcEngine::UUID ,std::string> m_map;

static void AddToMap(){
    m_map[RcEngine::UUID()] = "RcEngine";
}

namespace RcEngine{
    static std::random_device s_RandomDevice;
    static std::mt19937_64 s_Engine(s_RandomDevice());
    static std::uniform_int_distribution<uint32_t> s_UniformDistribution;

    UUID::UUID()
    : m_UUID(s_UniformDistribution(s_Engine)){

    }

    UUID::UUID(uint64_t uuid)
        : m_UUID(uuid){

    }
}