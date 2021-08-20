//
// Created by Tristan Zippert on 6/6/21.
//
#pragma once

#ifndef RCENGINE_CLION_CAMERA_H
#define RCENGINE_CLION_CAMERA_H

#include <glm/glm.hpp>

namespace RcEngine{
    class Camera{
    public:
        Camera() = default;

        Camera(const glm::mat4& projection)
            :m_Projection(projection){}
        virtual ~Camera() = default;
        const glm::mat4& GetProjection() const{return m_Projection;}

    protected:
        glm::mat4  m_Projection = glm::mat4(1.0f);

    };
}

#endif //RCENGINE_CLION_CAMERA_H
