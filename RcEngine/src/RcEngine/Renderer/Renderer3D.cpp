//
// Created by Tristan Zippert on 7/28/21.
//

#include "Renderer3D.h"
namespace RcEngine{
    struct CubeVertex{
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;
    };


    struct Renderer3DStorage{
        Ref<VertexArray> CubeVertexArray;
        Ref<VertexBuffer> CubeIndexBuffer;

        uint32_t  CubeIndexCount = 0;



    };

    static Renderer3DStorage s_Data;

    void Renderer3D::Init() {

    }

    void Renderer3D::Shutdown() {

    }

    void Renderer3D::BeginScene(const EditorCamera &camera) {

    }

    void Renderer3D::BeginScene(const Camera &camera, const glm::mat4 transform) {

    }

    void Renderer3D::EndScene() {

    }

    void Renderer3D::Flush() {

    }
}
