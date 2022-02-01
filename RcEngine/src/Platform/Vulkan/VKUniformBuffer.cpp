//
// Created by Tristan Zippert on 11/24/21.
//

#include "VKUniformBuffer.h"

namespace RcEngine{
    VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size, uint32_t binding) {
        bindingDescription.binding = binding;
        bindingDescription.stride = size;
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;



    }
    VulkanUniformBuffer::~VulkanUniformBuffer() {

    }

}