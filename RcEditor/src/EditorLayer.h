//
// Created by Tristan Zippert on 7/6/21.
//
#pragma once
#include "RcEngine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

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
        bool OnKeyPressed(KeyPressedEvent& e );

        void NewScene();
        void OpenScene();
        void SaveAs();
    private:
        OrthoCameraController m_CameraController;

        Ref<RcEngine::Shader> m_FlatColorShader;
        Ref<RcEngine::VertexArray> m_SquareVA;
        Ref<RcEngine::FrameBuffer> m_FrameBuffer;

        float m_TextureTile = 1.0f, m_Rotation =0.0f;

        glm::vec4  m_SquareColor = {0.8f,0.0f,0.0f,1.0f};
        glm::vec4  m_TextureColor = {0.2f,0.3f,0.8f,1.0f};

        EditorCamera m_EditorCamera;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];

        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCamera;
        Ref<Scene> m_ActiveScene;

        struct ProfileResult{
            const char* Name;
            float time;
        };
        std::vector<ProfileResult> m_Profiler;


        glm::vec2 m_ViewPortSize = {1264,666};

        //panels
        SceneHierarchyPanel m_Panel;
        ContentBrowserPanel m_ContentBrowserPanel;

        //Script
        std::string m_ScriptString;

        int m_GizmoType = -1;

    };
}
#endif //RCENGINE_CLION_EDITORLAYER_H
