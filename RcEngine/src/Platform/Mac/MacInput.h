//
// Created by Tristan Zippert on 5/24/21.
//

#pragma once
#include "RcEngine/Core/Input.h"
namespace RcEngine{
    class MacInput: public Input{
    protected:
        virtual bool IsKeyPressedImpl(int keycode) override;


        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
        virtual std::pair<float,float> GetMousePosImpl() override;

        virtual bool IsMouseButtonPressedImpl(int button) override;
    };
}