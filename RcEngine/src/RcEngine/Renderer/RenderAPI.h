//
// Created by Tristan Zippert on 5/30/21.
//

#pragma once
#include <glm.hpp>
#include "VertexArray.h"

namespace RcEngine{
    class RenderAPI{
    public:
        enum class API{
            None=0,
            OpenGL = 1,
            Vulkan = 2,
            Metal = 3,
        };

    public:
        virtual void Init() =0;
        virtual void Clear() =0;

        virtual void SetClearColor(const glm::vec4 color) =0;
        virtual void SetViewport(uint32_t x, uint32_t y,uint32_t width,uint32_t height) =0;

        virtual void DrawIndexed(const Ref<VertexArray>&  vertexArray, uint32_t indexCount =0) =0;
        inline static API GetAPI(){return s_API;}
        inline static void SetAPI(API api){s_API = api;}
    private:
        static API s_API;
    };
}