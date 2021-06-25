//
// Created by Tristan Zippert on 6/17/21.
//

#include "OpenGLShader.h"
#include "rcpch.h"


#include <external/glad/include/glad/glad.h>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include "Platform/OpenGL/OpenGLUtils.h"


namespace RcEngine{
    static GLenum ShaderTypeFromString(const std::string& type){
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
        RC_CORE_ASSERT(false, "unknown shader type");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string &filepath) {

        std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        // Assets/Shaders/TextureCombined.glsl
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash+1;
        auto lastDot = filepath.rfind('.');

        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;

        m_Name = filepath.substr(lastSlash,count);
    }
    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source){
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken,0);
        while(pos != std::string::npos){
            size_t eol = source.find_first_of("\r\n",pos);
            RC_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength +1;
            std::string type = source.substr(begin,eol - begin);

            size_t nextLinePos = source.find_first_not_of("\r\n",eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[ShaderTypeFromString(type)] =
                    source.substr(nextLinePos,
                                  pos - (nextLinePos == std::string::npos ? source.size() - 1: nextLinePos));
        }
        return shaderSources;
    }
    std::string OpenGLShader::ReadFile(const std::string &filepath) {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);

        if(in){
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0,std::ios::beg);
            in.read(&result[0],result.size());
        }else{
            RC_CORE_ERROR("Could not open file {0}", filepath);
        }
        return result;
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string &vertexSource, const std::string &fragmentSource)
    : m_Name(name){
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSource;
        sources[GL_FRAGMENT_SHADER] = fragmentSource;
        Compile(sources);
    }
    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> &shaderSource) {
        GLuint program = glCreateProgram();
        RC_CORE_ASSERT(shaderSource.size() <= 2, "Only 3 shaders are supported");
        std::array<GLenum,3> glShaderIDs;
        int glShaderIDIndex =0;
        for(auto& kv_pair : shaderSource){
           GLenum type = kv_pair.first;
           const std::string& sourceBase = kv_pair.second;

           GLuint shader = glCreateShader(type);

           const GLchar *source = sourceBase.c_str();
           glShaderSource(shader, 1, &source, 0);

            // Compile the vertex shader
           glCompileShader(shader);

           GLint isCompiled = 0;
           glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
           if(isCompiled == GL_FALSE)
           {
               GLint maxLength = 0;
               glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

               std::vector<GLchar> infoLog(maxLength);
               glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

               glDeleteShader(shader);

               RC_CORE_ERROR("Vertex shader compilation error!");
               RC_CORE_ERROR("{0}", infoLog.data());
               RC_CORE_ASSERT(false, "Shader compilation issue");

               break;
           }
           glAttachShader(program, shader);
           glShaderIDs[glShaderIDIndex++] = shader;
       }

        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked != 1)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);

            for(auto id: glShaderIDs)
                glDeleteShader(id);

            RC_CORE_ERROR("Linking shader error!");
            RC_CORE_ERROR("{0}", infoLog.data());
            RC_CORE_ASSERT(false, "Shader linking issue");
            return;
        }
        for (auto id: glShaderIDs)
            glDetachShader(program,id);

        m_RendererID = program;
    }
    void OpenGLShader::Bind() const {
        glUseProgram(m_RendererID);
    }
    void OpenGLShader::Unbind() const {
        glUseProgram(0);
    }
    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(m_RendererID);
    }
    void OpenGLShader::SetInt(const std::string &name, const int value) {
        UploadUniformInt(name,value);
    }
    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3 &value) {
        UploadUniformFloat3(name,value);
    }
    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4 &value) {
        UploadUniformFloat4(name,value);
    }
    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4 value) {
        UploadUniformMat4(name,value);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, const int value){
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID,name.c_str());
        glUniform1i(location,value);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix){
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID,name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID,name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, const float values){
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, values);
    }
    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values){
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, values.x, values.y);
    }
    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values){
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string &name, const glm::vec4& values) {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::UploadSampler(const char * name){
        glUniform1i(glGetUniformLocation(m_RendererID, name), 0);
    }
}