//
// Created by Tristan Zippert on 7/5/21.
//
#pragma once
#include "RcEngine/Core/Core.h"

#ifndef RCENGINE_CLION_FRAMEBUFFER_H
#define RCENGINE_CLION_FRAMEBUFFER_H
namespace RcEngine{
    struct FrameBufferSpec
    {
        uint32_t Width, Height;
        // FramebufferFormat Format =
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class FrameBuffer
    {
    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        virtual void Resize(uint32_t width, uint32_t height) =0;

        virtual uint32_t GetColorAttachmentRendererID() const = 0;

        virtual uint32_t GetRendererID() const = 0;

        virtual const FrameBufferSpec& GetSpecification() const = 0;

        static Ref<FrameBuffer> Create(const FrameBufferSpec& spec);
    };

}

#endif //RCENGINE_CLION_FRAMEBUFFER_H
