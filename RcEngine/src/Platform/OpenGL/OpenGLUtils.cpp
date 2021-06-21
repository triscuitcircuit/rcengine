//
// Created by Tristan Zippert on 6/8/21.
//

#include "OpenGLUtils.h"
#include <include/glad/glad.h>

namespace RcEngine{

    OpenGLUtils::OpenGLUtils() {}

    void OpenGLUtils::PrintShaderLog(GLuint shader){
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
    bool OpenGLUtils::CheckOpenGLError() {
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
    void OpenGLUtils::printProgramLog(int prog) {
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
    void OpenGLUtils::displayComputerShaderLimits(){
        int work_grp_cnt[3];
        int work_grp_siz[3];
        int work_grp_inv;
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_siz[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_siz[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_siz[2]);
        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
        printf("maximum number of workgroups is: %i  %i  %i\n", work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);
        printf("maximum size of workgroups is: %i  %i  %i\n", work_grp_siz[0], work_grp_siz[1], work_grp_siz[2]);
        printf("max local work group invocations %i\n", work_grp_inv);
    }
    std::string OpenGLUtils::readShaderFile(const char *filepath) {
        using namespace std;
        string content;
        ifstream fileStream(filepath, ios::in);
        string line = "";
        if(!fileStream){
            cout << "Couldn't find file: " << filepath<< endl;
            return "";
        }else{
            while (!fileStream.ios_base::eof()) {
                getline(fileStream, line);
                content.append(line+"\n");
            }
        }
        fileStream.close();
        return content;
    }
}