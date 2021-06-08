//
// Created by Tristan Zippert on 6/8/21.
//

#include <glm.hpp>

#include <iostream>
#include <fstream>
#include <external/GLFW/include/GLFW/glfw3.h>
#include <include/glad/glad.h>
#include "Utils.h"

namespace RcEngine{
    Utils::Utils() {}
    void Utils::PrintShaderLog(GLuint shader){
        int len =0;
        int chwritten =0;
        char* log;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH, &len);
        if(len> 0){
            log = (char*)malloc(len);
            glGetShaderInfoLog(shader,len,&chwritten,log);
            std::cout << "Shader log info: " << log << std::endl;
            free(log);
        }
    }
    bool Utils::CheckOpenGLError() {
        bool found_Err = false;
        int glErr = glGetError();
        while(glErr != GL_NO_ERROR){
            std::cout << "glError: "<< glErr << std::endl;
//            std::cout << "GLStrinError: " <<
            found_Err = true;
            glErr = glGetError();
        }
        return found_Err;
    }
    void Utils::printProgramLog(int prog) {
        int len = 0, chWritten =0;
        char *log;
        glGetProgramiv(prog,GL_INFO_LOG_LENGTH,&len);
        if (len > 0){
            log = (char*)malloc(len);
            glGetProgramInfoLog(prog,len,&chWritten,log);
            std::cout << "Program Log: " << log << std::endl;
            free(log);
        }
    }
}