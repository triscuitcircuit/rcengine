//
// Created by Tristan Zippert on 7/30/22.
//

#ifndef RCGAMEENGINE_MESHLOADER_H
#define RCGAMEENGINE_MESHLOADER_H
#include <tiny_obj_loader.h>
namespace RcEngine{
    class Mesh{
        Mesh(char* filename);
        ~Mesh() = default;
        bool MeshLoad();
    private:
        char* m_filename;
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
    };
}
#endif //RCGAMEENGINE_MESHLOADER_H
