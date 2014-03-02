#include "renderstage.h"

RenderStage::RenderStage()
{
    loaded = false;
};

void RenderStage::init(int w, int h)
{
    //TEXTURE
    //glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &fbo_texture);
//    cout<<"fbo_texture id: "<<fbo_texture<<"\n";
    glBindTexture(GL_TEXTURE_2D, fbo_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    //DEPTH BUFFER (RENDER)
//      glGenRenderbuffers(1, &rbo_depth_blur[i]);
//      glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
//      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, w, h);
//      glBindRenderbuffer(GL_RENDERBUFFER, 0);

    //DEPTH BUFFER (TEXTURE)
    glGenTextures(1, &fbo_depth);
//    cout<<"fbo_depth id: "<<fbo_depth<<"\n";
    glBindTexture(GL_TEXTURE_2D, fbo_depth);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        //NULL means reserve texture memory, but texels are undefined
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    //FRAMEBUFFER2
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
        //Attach depth texture to FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo_depth, 0/*mipmap level*/);

        //Depth Renderbuffer
        //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
        GLenum status;
        if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
        {
            fprintf(stderr, "glCheckFramebufferStatus original image: error %p", status);
            std::cout<<"\n";
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    loaded = true;
}

void RenderStage::resize(int w, int h)
{
    glBindTexture(GL_TEXTURE_2D, fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, fbo_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glViewport(0, 0, w, h);
}

void RenderStage::activate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void RenderStage::deactivate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

RenderStage::~RenderStage()
{
    if (loaded)
    {
        cout<<"deleting rendering stage buffers\n";
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteTextures(1, &fbo_texture);
        glDeleteFramebuffers(1, &fbo);
        glDeleteRenderbuffers(1, &fbo_depth);
    }
}
