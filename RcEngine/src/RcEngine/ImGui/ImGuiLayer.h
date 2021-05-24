//
// Created by Tristan Zippert on 5/23/21.
//


#include "RcEngine/Layer.h"

namespace RcEngine{
    class RC_API ImGuiLayer : public Layer{

    private:
        float m_Time =0.0f;
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();
        void OnUpdate();
        void OnEvent(Event& event);
    };
}