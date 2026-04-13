//
// Created by Tristan Zippert on 6/24/21.
//
#pragma once
#include "RcEngine.h"

#include "ParticleSystem.h"

#ifndef RCENGINE_CLION_RCGAME2D_H
#define RCENGINE_CLION_RCGAME2D_H
class RcGame2D : public RcEngine::Layer{
public:
    RcGame2D();
    virtual ~RcGame2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(RcEngine::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(RcEngine::Event& e) override;
private:
    RcEngine::OrthoCameraController m_CameraController;

    RcEngine::Ref<RcEngine::Shader> m_FlatColorShader;
    RcEngine::Ref<RcEngine::VertexArray> m_SquareVA;

    float m_TextureTile = 1.0f, m_Rotation =0.0f;

    glm::vec4  m_SquareColor = {0.8f,0.0f,0.0f,1.0f};
    glm::vec4  m_TextureColor = {0.2f,0.3f,0.8f,1.0f};

    ParticleSystem m_ParticleSystem;
    ParticleProps m_Particle;

    RcEngine::Ref<RcEngine::Texture2D> m_BaseTexture;
    RcEngine::Ref<RcEngine::Texture2D> m_SpriteSheet;
    RcEngine::Ref<RcEngine::Texture2D> m_BackgroundSheet;
    RcEngine::Ref<RcEngine::SubTexture2D> m_Sprite, m_Background;

    struct ProfileResult{
        const char* Name;
        float time;
    };
    std::vector<ProfileResult> m_Profiler;

    uint32_t m_MapWidth,m_MapHeight;

    std::unordered_map<char,RcEngine::Ref<RcEngine::SubTexture2D>> s_TextureMap;

};
#endif //RCENGINE_CLION_RCGAME2D_H
