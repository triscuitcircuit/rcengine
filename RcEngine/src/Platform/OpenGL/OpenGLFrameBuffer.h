//
// Created by Tristan Zippert on 7/5/21.
//
#pragma once
#include "RcEngine/Renderer/FrameBuffer.h"

#ifndef RCENGINE_CLION_OPENGLFRAMEBUFFER_H
#define RCENGINE_CLION_OPENGLFRAMEBUFFER_H
namespace RcEngine {
    class OpenGLFramebuffer : public FrameBuffer {
    public:
        OpenGLFramebuffer(const FrameBufferSpec &spec);

        virtual ~OpenGLFramebuffer();

        void Invalidate();

        virtual void Bind() override;

        virtual void UnBind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;

        virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

        virtual uint32_t GetRendererID() const override{return m_RendererID;}

        virtual const FrameBufferSpec &GetSpecification() const override { return m_Specification; }

    private:
        uint32_t m_RendererID =0;
        uint32_t m_ColorAttachment =0, m_DepthAttachment=0;
        FrameBufferSpec m_Specification;
    };

}
#endif //RCENGINE_CLION_OPENGLFRAMEBUFFER_H
