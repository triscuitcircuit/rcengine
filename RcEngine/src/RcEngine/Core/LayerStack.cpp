//
// Created by Tristan Zippert on 5/23/21.
//

#include "LayerStack.h"
#include "rcpch.h"

namespace RcEngine{

    LayerStack::LayerStack(){
    }

    LayerStack::~LayerStack(){
        for (Layer* layer :m_Layers)
            delete layer;
    }
    void LayerStack::PushLayer(Layer* layer){
        m_Layers.emplace(m_LayerInsertIndex + m_Layers.begin(), layer);
        m_LayerInsertIndex++;
    }
    void LayerStack::PopLayer(Layer *layer) {
        auto deletion = std::find(m_Layers.begin(),m_Layers.end(),layer);
        if(deletion != m_Layers.end()){
            m_Layers.erase(deletion);
            m_LayerInsertIndex--;
        }
    }
    void LayerStack::PushOverlay(Layer *overlay) {
        //back to front starting at the main game layer
        m_Layers.emplace_back(overlay);
    }
    void LayerStack::PopOverlay(Layer *overlay) {
        auto delayer = std::find(m_Layers.begin(),m_Layers.end(),overlay);
        if(delayer != m_Layers.end())
            m_Layers.erase(delayer);
    }
}