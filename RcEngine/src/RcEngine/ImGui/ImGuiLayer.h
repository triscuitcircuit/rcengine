//
// Created by Tristan Zippert on 5/23/21.
//

#pragma once
#include "RcEngine/Core/Layer.h"


#include "RcEngine/Events/MouseEvent.h"
#include "RcEngine/Events/ApplicationEvent.h"
#include "RcEngine/Events/KeyEvent.h"

namespace RcEngine{
    class RC_API ImGuiLayer : public Layer{

    private:
        float m_Time =0.0f;

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnImGuiRender() override;

        void Begin();
        void End();

        virtual void SetDarkThemeColors();
    };
}