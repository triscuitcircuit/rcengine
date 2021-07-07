//
// Created by Tristan Zippert on 7/6/21.
//
#pragma once
#include "RcEngine.h"

#ifndef RCENGINE_CLION_EDITORLAYER_H
#define RCENGINE_CLION_EDITORLAYER_H
namespace RcEngine{
    class EditorLayer : public Layer{
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;
        void OnEvent(Event& e) override;
    private:
        RcEngine::OrthoCameraController m_CameraController;

        RcEngine::Ref<RcEngine::Shader> m_FlatColorShader;
        RcEngine::Ref<RcEngine::VertexArray> m_SquareVA;
        RcEngine::Ref<RcEngine::FrameBuffer> m_FrameBuffer;

        float m_TextureTile = 1.0f, m_Rotation =0.0f;

        glm::vec4  m_SquareColor = {0.8f,0.0f,0.0f,1.0f};
        glm::vec4  m_TextureColor = {0.2f,0.3f,0.8f,1.0f};


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

        glm::vec2 m_ViewPortSize = {1264,666};

        std::unordered_map<char,RcEngine::Ref<RcEngine::SubTexture2D>> s_TextureMap;

    };
}
#endif //RCENGINE_CLION_EDITORLAYER_H
