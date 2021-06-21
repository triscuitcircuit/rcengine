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
            Vulkan = 2
        };
    public:
        virtual void Clear() =0;
        virtual void SetClearColor(const glm::vec4 color) =0;

        virtual void DrawIndexed(const Ref<VertexArray>&  vertexArray) =0;
        inline static API GetAPI(){return s_API;};
    private:
        static API s_API;
    };
}