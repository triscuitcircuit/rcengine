//
// Created by Tristan Zippert on 7/5/21.
//
#pragma once
#include "RcEngine/Core/Core.h"

#ifndef RCENGINE_CLION_FRAMEBUFFER_H
#define RCENGINE_CLION_FRAMEBUFFER_H
namespace RcEngine{
    enum class FrameBufferTextureFormat{
        None =0,
        RGBA8 = 1,
        RED_INT,
        RGBA16F,
        //stencil
        DEPTH24STENCIL8,
        //Defaults
        Depth = DEPTH24STENCIL8
    };
    struct FramebufferTextureSpec{
        FramebufferTextureSpec() = default;
        FramebufferTextureSpec(FrameBufferTextureFormat format)
            : TextureFormat(format){}

        FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
    };
    struct FramebufferAttachmentSpec{
        FramebufferAttachmentSpec() =default;
        FramebufferAttachmentSpec(std::initializer_list<FramebufferTextureSpec> attachments)
        : Attachments(attachments){}
        std::vector<FramebufferTextureSpec> Attachments;
    };

    struct FrameBufferSpec
    {
        uint32_t Width, Height;
        FramebufferAttachmentSpec Attachments;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class FrameBuffer
    {
    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        virtual int ReadPixel(uint32_t attachmentidx, int x, int y) =0;

        virtual void Resize(uint32_t width, uint32_t height) =0;

        virtual uint32_t GetColorAttachmentRendererID(uint32_t index=0) const = 0;



        virtual uint32_t GetRendererID() const = 0;

        virtual const FrameBufferSpec& GetSpecification() const = 0;

        static Ref<FrameBuffer> Create(const FrameBufferSpec& spec);
    };

}

#endif //RCENGINE_CLION_FRAMEBUFFER_H
