#include "shadowmap.h"

ShadowMap::ShadowMap() :
    fbo(0),
    tex_depth(0),
    light_mvp_mat(0)
{
    //ctor
}

void ShadowMap::init()
{
     // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    fbo = 0;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    GLuint tex_depth;
    glGenTextures(1, &tex_depth);
    glBindTexture(GL_TEXTURE_2D, tex_depth);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex_depth, 0);

    // No color output in the bound framebuffer, only depth.
    glDrawBuffer(GL_NONE);

    // what is a readbuffer?
    glReadBuffer(GL_NONE); /// Why is this needed?

    // Always check that our framebuffer is ok
    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "glCheckFramebufferStatus original image: error %p", status);
        std::cout<<"\n";
    }

    // Create and compile our GLSL program from the shaders
    ShaderBase::load("shaders/shadow_map_vert.glsl", "shaders/shadow_map_frag.glsl" );

    // Get a handle for our "depthMVP" uniform
    light_mvp_mat  = glGetUniformLocation(getProgramID(), "depthMVP");
}

void ShadowMap::activate()
{
    // Render to our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0,0,1024,1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderBase::switchTo();

    glm::vec3 lightInvDir(1.0, 1.0, 1.0);

    // Compute the MVP matrix from the light's point of view
    depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
    depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
    // or, for spot light :
    //glm::vec3 lightPos(5, 20, 20);
    //glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
    //glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));


}

void ShadowMap::deactivate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ShadowMap::drawActor(shared_ptr<Actor> actor, glm::mat4 mv)
{

}

void ShadowMap::drawProp(shared_ptr<Prop> prop, glm::mat4 mv)
{

}

ShadowMap::~ShadowMap()
{
    if (isLoaded())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteTextures(1, &tex_depth);
        glDeleteFramebuffers(1, &fbo);
    }

    ShaderBase::unload();
}
