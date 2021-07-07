#include "rcpch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace RcEngine{

    static const uint32_t s_MaxFrameBufferSize = 16384;

    OpenGLFramebuffer::OpenGLFramebuffer(const FrameBufferSpec& spec)
            : m_Specification(spec)
    {
        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if(m_RendererID){
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }

        glGenFramebuffers(1,&m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER,m_RendererID);

        glGenTextures(1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D,m_ColorAttachment);

        glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA32F, m_Specification.Width,
                     m_Specification.Height,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_ColorAttachment,0);

        glGenTextures(1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D,m_DepthAttachment);

        glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH24_STENCIL8,m_Specification.Width,m_Specification.Height,0,
                     GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_TEXTURE_2D,m_DepthAttachment,0);

        RC_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,"Framebuffer incomplete");

        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0,0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGLFramebuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {

        if(width ==0 || height == 0|| width >  s_MaxFrameBufferSize|| height > s_MaxFrameBufferSize){
            RC_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
            return;
        }

        m_Specification.Width = width;
        m_Specification.Height = height;
        Invalidate();
    }
}