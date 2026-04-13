//
// Created by Tristan Zippert on 11/24/21.
//

#ifndef RCENGINE_VKUNIFORMBUFFER_H
#define RCENGINE_VKUNIFORMBUFFER_H

#include "RcEngine/Renderer/UniformBuffer.h"
#include <vulkan/vulkan_core.h>

namespace RcEngine{
    class VulkanUniformBuffer: public UniformBuffer{
        VulkanUniformBuffer(uint32_t size, uint32_t binding);
        virtual ~VulkanUniformBuffer();

        virtual void SetData(const void* data, uint32_t size, uint32_t offset =0) override;

    private:
        VkVertexInputBindingDescription bindingDescription;
    };
}
#endif //RCENGINE_VKUNIFORMBUFFER_H
