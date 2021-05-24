//
// Created by Tristan Zippert on 5/23/21.
//
#pragma once
#include "RcEngine/Core/Core.h"
#include "Layer.h"
#include <vector>

namespace RcEngine{
    class RC_API LayerStack{
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() {
            return m_Layers.begin();
        }
        std::vector<Layer*>::iterator end() {return m_Layers.end();}
    private:
        std::vector<Layer*> m_Layers;
        std::vector<Layer*>::iterator m_LayerInsert;
    };
}