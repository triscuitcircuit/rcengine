//
// Created by Tristan Zippert on 6/19/21.
//
#pragma once
#include "RcEngine/Core/Core.h"

#ifndef RCENGINE_CLION_TEXTURE_H
#define RCENGINE_CLION_TEXTURE_H

namespace RcEngine{
class Texture{
public:
    virtual ~Texture() = default;
    virtual uint32_t  GetWidth() const =0;
    virtual uint32_t GetHeight() const= 0;

    virtual uint32_t GetRendererID() const =0;

    virtual void Bind(uint32_t slot = 0) const =0;
    virtual void SetData(void* data, uint32_t size, uint32_t slot =0) const =0;
    virtual std::string getPath() const =0;

    virtual bool operator==(const Texture& otherTexture) const = 0;
protected:
    static std::string m_Path;
};
    class Texture2D: public Texture{
    public:
        static Ref<Texture2D> Create(const char* path);
        static Ref<Texture2D> Create(uint32_t width, uint32_t height);
    };
}

#endif //RCENGINE_CLION_TEXTURE_H
