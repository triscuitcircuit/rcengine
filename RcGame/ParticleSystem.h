//
// Created by Tristan Zippert on 7/2/21.
//
#pragma once
#include "RcEngine.h"

#ifndef RCENGINE_CLION_PARTICLESYSTEM_H
#define RCENGINE_CLION_PARTICLESYSTEM_H
struct ParticleProps{
    glm::vec2 Position;
    glm::vec2 Velocity, VelocityVariation;
    glm::vec4 ColorBegin, ColorEnd;
    float SizeBegin, SizeEnd, SizeVariation;
    float LifeTime = 4.0f;
};
class ParticleSystem{
public:
    ParticleSystem(uint32_t maxParticles= 100000);

    void OnUpdate(RcEngine::Timestep ts);
    void OnRender(RcEngine::OrthoCamera& camera);

    void Emit(const ParticleProps& particleProps);
private:
    struct Particle{
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec4 ColorBegin, ColorEnd;
        float Rotation = 0.0f;
        float SizeBegin, SizeEnd;

        float LifeTime = 1.0f;
        float LifeRemaining = 0.0f;

        bool Active = false;
    };
    std::vector<Particle> m_ParticlePool;
    uint32_t m_PoolIndex =0;

    GLuint m_QuadVA = 0;
    std::unique_ptr<RcEngine::Shader> m_ParticleShader;
    GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
};


#endif //RCENGINE_CLION_PARTICLESYSTEM_H
