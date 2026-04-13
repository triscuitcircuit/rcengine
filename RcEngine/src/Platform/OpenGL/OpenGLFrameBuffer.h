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

        virtual int ReadPixel(uint32_t attachmentidx, int x, int y) override;


        virtual void Resize(uint32_t width, uint32_t height) override;

        virtual uint32_t GetColorAttachmentRendererID(uint32_t index) const override {
            RC_CORE_ASSERT(index < m_ColorAttachments.size(),"");
            return m_ColorAttachments[index]; }

        virtual uint32_t GetRendererID() const override{return m_RendererID;}

        virtual const FrameBufferSpec &GetSpecification() const override { return m_Specification; }

    private:
        uint32_t m_RendererID =0;
        FrameBufferSpec m_Specification;

        std::vector<FramebufferTextureSpec> m_ColorAttachmentSpecs;
        FramebufferTextureSpec m_DepthAttachmentSpec = FrameBufferTextureFormat::None;

        std::vector<RendererID> m_ColorAttachments;
        uint32_t  m_DepthAttachment = 0;
    };

}
#endif //RCENGINE_CLION_OPENGLFRAMEBUFFER_H
