//
// Created by Tristan Zippert on 5/23/21.
//
#pragma once
#include "RcEngine/Core/Core.h"
#include "RcEngine/Events/Event.h"

namespace RcEngine{
    class RC_API Layer{
    public:
        Layer(const std::string& name = "RCLayer");
        virtual ~Layer();

        virtual void OnAttach(){}
        virtual void OnDetach(){}
        virtual void OnUpdate(){}
        virtual void OnEvent(Event& event){}

        inline const std::string& GetName() const{return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}
