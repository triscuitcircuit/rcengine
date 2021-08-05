//
// Created by Tristan Zippert on 8/2/21.
//
#pragma once

#include "RcEngine/Renderer/UniformBuffer.h"

#ifndef RCENGINE_OPENGLUNIFORMBUFFER_H
#define RCENGINE_OPENGLUNIFORMBUFFER_H
namespace RcEngine{
    class OpenGlUniformBuffer: public UniformBuffer{
    public:
        OpenGlUniformBuffer(uint32_t size, uint32_t binding);
        virtual ~OpenGlUniformBuffer();

        virtual void SetData(const void* data, uint32_t size, uint32_t offset =0) override;
    private:
        uint32_t m_RendererID =0;
    };
}
#endif //RCENGINE_OPENGLUNIFORMBUFFER_H
