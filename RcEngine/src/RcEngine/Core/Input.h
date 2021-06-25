//
// Created by Tristan Zippert on 5/24/21.
//

#pragma once

#include "RcEngine/Core/Core.h"

namespace RcEngine{
    class RC_API Input{
    public:
        inline static bool IsKeyPressed(int keycode){return s_Instance->IsKeyPressedImpl(keycode);}

        inline static bool IsMouseButtonPressed(int button){return s_Instance->IsMouseButtonPressedImpl(button);}
        inline static float GetMouseX() {return s_Instance-> GetMouseXImpl();}
        inline static float GetMouseY() {return s_Instance->GetMouseYImpl(); }
        inline static std::pair<float,float> GetMousePos() {return s_Instance->GetMousePosImpl(); }
    protected:
        virtual bool IsKeyPressedImpl(int keycode) =0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual float GetMouseXImpl()=0;
        virtual float GetMouseYImpl()=0;
        virtual std::pair<float,float> GetMousePosImpl()=0;


    private:
        static Input* s_Instance;
    };
}