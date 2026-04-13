//
// Renderer3D - 3D mesh rendering with Phong lighting (OpenGL 4.1)
//
#pragma once

#include "Camera.h"
#include "Texture.h"
#include "ProjectionCamera.h"
#include "EditorCamera.h"
#include "Mesh.h"

#include <glm/glm.hpp>

namespace RcEngine {

    class Renderer3D {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const ProjectionCamera& camera);
        static void BeginScene(const EditorCamera& camera);
        static void BeginScene(const Camera& camera, const glm::mat4& transform);

        static void EndScene();

        // Draw an OBJ mesh with a flat color and optional Phong shading
        static void DrawMesh(const glm::mat4& transform, Ref<Mesh> mesh,
                             const glm::vec4& color = glm::vec4(1.0f),
                             int entityID = -1);

        // Draw an OBJ mesh with a texture
        static void DrawMesh(const glm::mat4& transform, Ref<Mesh> mesh,
                             Ref<Texture2D> texture,
                             int entityID = -1);

        // Draw a unit-cube primitive (useful for CubeComponent / box collider visualisation)
        static void DrawCube(const glm::mat4& transform,
                             const glm::vec4& color = glm::vec4(1.0f),
                             int entityID = -1);

        struct Stats {
            uint32_t DrawCalls = 0;
            uint32_t MeshCount = 0;
        };
        static Stats GetStats();
        static void ResetStats();
    };

}
