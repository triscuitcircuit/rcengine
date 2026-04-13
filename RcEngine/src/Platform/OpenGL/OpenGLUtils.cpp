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
            RC_CORE_INFO("Shader log info: {0}",log);
            free(log);
        }
    }
    bool OpenGLUtils::CheckOpenGLError() {
        bool found_Err = false;
        int glErr = glGetError();
        while(glErr != GL_NO_ERROR){
            RC_CORE_ERROR("OpenGL error: {0}", glErr);
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
            RC_CORE_INFO("Program Log: {0}",log);
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

        RC_CORE_INFO("maximum number of workgroups is:{0} {1} {2}",
                     work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]
                     );

        RC_CORE_INFO("maximum size of workgroups is:{0} {1} {2}",
                     work_grp_siz[0], work_grp_siz[1], work_grp_siz[2]
        );

        RC_CORE_INFO("max local work group invocations:{0}",work_grp_inv);
    }
    std::string OpenGLUtils::readShaderFile(const char *filepath) {
        using namespace std;
        string content;
        ifstream fileStream(filepath, ios::in);
        string line = "";
        if(!fileStream){
            RC_CORE_WARN("Couldnt find file: {0}",filepath);
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