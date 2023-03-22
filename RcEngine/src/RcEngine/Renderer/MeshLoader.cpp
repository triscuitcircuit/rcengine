//
// Created by Tristan Zippert on 7/30/22.
//

#include "MeshLoader.h"
#include "vulkan/vulkan.h"

namespace RcEngine{
    Mesh::Mesh(char *filename): m_filename(filename) {};

    bool Mesh::MeshLoad() {
        std::string err;
        std::string warn;

        tinyobj::LoadObj(&attrib,&shapes,&materials,&warn,&err,m_filename, nullptr);

        if(!warn.empty())
            RC_WARN(warn);

        if(!err.empty())
            RC_CORE_ASSERT(!err.empty(),err);
            return false;

        for (size_t s = 0; s < shapes.size(); s ++) {
            size_t index_offset = 0;
            for(size_t f =0; f< shapes[s].mesh.num_face_vertices.size();f++){
                int fv =3;

                for(size_t v=0; v < fv; v++){
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset +v];

                    tinyobj::real_t ux = attrib.vertices[3 * idx.vertex_index +0];
                    tinyobj::real_t uy = attrib.vertices[3 * idx.vertex_index +1];
                    tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index +2];

                    tinyobj::real_t nx = attrib.normals[3 * idx.normal_index +0];
                    tinyobj::real_t ny = attrib.normals[3 * idx.normal_index +1];
                    tinyobj::real_t nz = attrib.normals[3 * idx.normal_index +2];



                }
            }
        }
    }
}
