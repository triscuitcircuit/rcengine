//
// Created by Tristan Zippert on 6/24/21.
//
#pragma once
#include "RcEngine.h"
#include "RcEngine/Renderer/OrthoCameraController.h"

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

    RcEngine::Ref<RcEngine::Texture2D> m_BaseTexture;

    struct ProfileResult{
        const char* Name;
        float time;
    };
    std::vector<ProfileResult> m_Profiler;

};
#endif //RCENGINE_CLION_RCGAME2D_H
