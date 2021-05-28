//
// Created by Tristan Zippert on 5/27/21.
//
#pragma once
namespace RcEngine{
    enum class RendererAPI{
        None=0,
        OpenGL = 1,
        Vulkan =2
    };
    class Renderer{
    public:
        inline static RendererAPI GetAPI(){return s_RendererAPI;}
        inline static void SetAPI(RendererAPI api){s_RendererAPI = api;}
    private:
        static RendererAPI s_RendererAPI;
    };
}
