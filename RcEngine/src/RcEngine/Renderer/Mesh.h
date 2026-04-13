//
// 3D Mesh class - loads OBJ files into GPU vertex/index buffers
//
#pragma once
#include "RcEngine/Core/Core.h"
#include "Buffer.h"
#include "VertexArray.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace RcEngine {

    struct MeshVertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    class Mesh {
    public:
        static Ref<Mesh> Create(const std::string& filepath);

        void Bind() const;
        uint32_t GetIndexCount() const { return m_IndexCount; }
        const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
        const std::string& GetFilePath() const { return m_FilePath; }
        bool IsLoaded() const { return m_Loaded; }

    private:
        Mesh() = default;
        bool Load(const std::string& filepath);

        Ref<VertexArray> m_VertexArray;
        uint32_t m_IndexCount = 0;
        std::string m_FilePath;
        bool m_Loaded = false;
    };

}
