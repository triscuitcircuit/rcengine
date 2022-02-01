//
// Created by Tristan Zippert on 1/23/22.
//

#ifndef RCENGINE_VKTEXTURE_H
#define RCENGINE_VKTEXTURE_H
#pragma once
#include "vulkan/vulkan_core.h"
#include "RcEngine/Renderer/Texture.h"


namespace RcEngine{
    struct UploadContext{
        VkFence _uploadFence;
        VkCommandPool _commandPool;
        VkCommandBuffer _commandBuffer;
    };
    class VKTexture2D : Texture2D{
    public:
        VKTexture2D(uint32_t width, uint32_t height);
        VKTexture2D(const char* path);


        virtual ~VKTexture2D();

        virtual void SetData(void* data, uint32_t size, uint32_t slot =0) const override;

        virtual uint32_t  GetHeight() const override{return m_Height;}
        virtual uint32_t  GetWidth() const override{return m_Width;}
        virtual uint32_t GetRendererID() const override {return m_RendererID;}

        virtual void Bind(uint32_t slot = 0) const override;

        virtual bool operator==(const Texture& other) const override{
            return m_RendererID == ((VKTexture2D&)other).m_RendererID;
        }
        virtual std::string getPath() const override{return m_Path;}

    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        UploadContext _uploadContext;
    };
}
#endif //RCENGINE_VKTEXTURE_H
