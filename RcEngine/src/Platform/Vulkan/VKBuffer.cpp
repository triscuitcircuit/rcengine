//
// Created by Tristan Zippert on 8/18/21.
//
#include "Platform/Vulkan/VKBuffer.h"
#include "vulkan/vulkan_core.h"

const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

namespace RcEngine{

}
