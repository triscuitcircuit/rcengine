//
// Created by Tristan Zippert on 8/2/21.
//
#pragma once

#include "RcEngine/Core/Core.h"

#ifndef RCENGINE_UNIFORMBUFFER_H
#define RCENGINE_UNIFORMBUFFER_H
namespace RcEngine{
    class UniformBuffer{
    public:
        virtual ~UniformBuffer(){};
        virtual void SetData(const void* data, uint32_t size, uint32_t offset =0) =0;

        static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
    };
}
#endif //RCENGINE_UNIFORMBUFFER_H
