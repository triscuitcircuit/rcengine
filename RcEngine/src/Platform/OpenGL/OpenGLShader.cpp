//
// Created by Tristan Zippert on 6/17/21.
//

#include "OpenGLShader.h"
#include "rcpch.h"


#include <external/glad/include/glad/glad.h>
#include <gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLUtils.h"


namespace RcEngine{
    OpenGLShader::OpenGLShader(const std::string &vertexSource, const std::string &fragmentSource) {
        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

// Send the vertex shader source code to GL
// Note that std::string's .c_str is NULL character terminated.
        const GLchar *source = vertexSource.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

// Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);



            // We don't need the shader anymore.
            glDeleteShader(vertexShader);

            RC_CORE_ERROR("Vertex shader compilation error!");
            RC_CORE_ERROR("{0}", infoLog.data());
            RC_CORE_ASSERT(false, "Vertex shader compilation issue");

            return;
        }

// Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

// Send the fragment shader source code to GL
// Note that std::string's .c_str is NULL character terminated.
        source = fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

// Compile the fragment shader
        glCompileShader(fragmentShader);
        OpenGLUtils::CheckOpenGLError();

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        OpenGLUtils::PrintShaderLog(fragmentShader);
        if (isCompiled != 1)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);

            RC_CORE_ERROR("Fragment shader compilation error!");
            RC_CORE_ERROR("{0}", infoLog.data());
            RC_CORE_ASSERT(false, "Fragment shader compilation issue");

            return;
        }

// Vertex and fragment shaders are successfully compiled.
// Now time to link them together into a program.
// Get a program object.
        m_RendererID = glCreateProgram();
        GLuint program = m_RendererID;
// Attach our shaders to our program
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

// Link our program
        glLinkProgram(program);

// Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
        OpenGLUtils::PrintShaderLog(vertexShader);
        if (isLinked != 1)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
            RC_CORE_ERROR("Linking shader error!");
            RC_CORE_ERROR("{0}", infoLog.data());
            RC_CORE_ASSERT(false, "Shader linking issue");
            return;
        }

// Always detach shaders after a successful link.
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
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