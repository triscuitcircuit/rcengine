//
// 3D Mesh implementation - OBJ loading via TinyObjLoader
//
#include "rcpch.h"
#include "Mesh.h"

#include <tiny_obj_loader.h>

namespace RcEngine {

    Ref<Mesh> Mesh::Create(const std::string& filepath) {
        auto mesh = std::shared_ptr<Mesh>(new Mesh());
        mesh->m_FilePath = filepath;
        mesh->m_Loaded = mesh->Load(filepath);
        return mesh;
    }

    bool Mesh::Load(const std::string& filepath) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
            RC_CORE_ERROR("Failed to load mesh: {0}", filepath);
            if (!err.empty()) RC_CORE_ERROR("OBJ Error: {0}", err);
            return false;
        }
        if (!warn.empty()) RC_CORE_WARN("OBJ Warning: {0}", warn);

        // Build vertex + index arrays, de-duplicating vertices
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;

        // Simple flat list (no deduplication to keep it straightforward)
        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                MeshVertex vertex{};

                vertex.Position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                if (index.normal_index >= 0 && !attrib.normals.empty()) {
                    vertex.Normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                }

                if (index.texcoord_index >= 0 && !attrib.texcoords.empty()) {
                    vertex.TexCoords = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1] // flip Y for OpenGL
                    };
                }

                indices.push_back(static_cast<uint32_t>(vertices.size()));
                vertices.push_back(vertex);
            }
        }

        if (vertices.empty()) {
            RC_CORE_ERROR("Mesh has no vertices: {0}", filepath);
            return false;
        }

        m_VertexArray = VertexArray::Create();

        auto vb = VertexBuffer::Create(static_cast<uint32_t>(vertices.size() * sizeof(MeshVertex)));
        vb->SetData(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(MeshVertex)));
        vb->SetLayout({
            { ShaderDataType::Float3, "a_Position"  },
            { ShaderDataType::Float3, "a_Normal"    },
            { ShaderDataType::Float2, "a_TexCoords" }
        });
        m_VertexArray->AddVertexBuffer(vb);

        auto ib = IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size()));
        m_VertexArray->SetIndexBuffer(ib);

        m_IndexCount = static_cast<uint32_t>(indices.size());

        RC_CORE_INFO("Loaded mesh '{0}': {1} vertices, {2} indices", filepath, vertices.size(), indices.size());
        return true;
    }

    void Mesh::Bind() const {
        m_VertexArray->Bind();
    }

}
