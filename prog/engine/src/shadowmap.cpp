#include "shadowmap.h"

ShadowMap::ShadowMap() :
    fbo(0),
    tex_depth(0),
    uniforms({0}),
    attributes({0}),
    light_vp_value(glm::mat4(1.0))
{

}

void ShadowMap::init()
{
    fbo = 0;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

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

    glDrawBuffer(GL_NONE);

    glReadBuffer(GL_NONE); // Why is this needed?

    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "glCheckFramebufferStatus original image: error %p", status);
        std::cout<<"\n";
    }

    ///Create and compile our GLSL program from the shaders
    ShaderBase::load("shaders/shadow_map_vert.glsl", "shaders/shadow_map_frag.glsl" );

    ///Uniforms
    uniforms.light_mvp_mat  = glGetUniformLocation(getProgramID(), "depthMVP");

    ///Attributes
    attributes.vertex = glGetAttribLocation(getProgramID(), "InVertex");
}

void ShadowMap::activate(const glm::mat4 &light_vp)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0,0,1024,1024);

    /// Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// switch shader program to this
    ShaderBase::switchTo();

    /// update the light view matrix
    light_vp_value = light_vp;
}

void ShadowMap::activate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0,0,1024,1024);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderBase::switchTo();

    glm::vec3 lightInvDir(1.0, 1.0, 1.0);

    // Compute the MVP matrix from the light's point of view
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));

    light_vp_value = depthProjectionMatrix * depthViewMatrix;

//    std::cout << "tex_depth = " << tex_depth << "\n";
}

void ShadowMap::activateDrawContent()
{
    glViewport(0,0,1024,1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderBase::switchTo();

    glm::vec3 lightInvDir(1.0, 1.0, 1.0);

    // Compute the MVP matrix from the light's point of view
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));

    light_vp_value = depthProjectionMatrix * depthViewMatrix;
}

void ShadowMap::deactivate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::drawActor(shared_ptr<Actor> actor)
{

}

void ShadowMap::drawProp(shared_ptr<Prop> prop)
{
    for (auto rb_it = prop->render_batches.begin(); rb_it!= prop->render_batches.end(); rb_it++)
    {
//        Mesh &mesh = *shared_ptr<Mesh>(*mesh_ptr_it); /// deref the it to ptr, from ptr to mesh
        shared_ptr<Mesh> mesh_ptr = shared_ptr<Mesh>(rb_it->mesh_ptr);
        glm::mat4 transf_mat = rb_it->transf_mat;

        /// set the modelview matrix for this model
        glm::mat4 tr = glm::translate(glm::mat4(1.0), prop->pos);
        glm::mat4 rt = glm::mat4_cast(prop->rot);
        glm::mat4 sc = glm::scale(glm::mat4(1.0), prop->scale);

        glm::mat4 mv = light_vp_value;
        glm::mat4 vertex_matrix  = mv * tr * rt * sc * transf_mat; // scale, then translate, then lookat.

        glUniformMatrix4fv(uniforms.light_mvp_mat, 1, false, &vertex_matrix[0][0]);

        /// Bind vertex data
        glBindBuffer(GL_ARRAY_BUFFER, mesh_ptr->getVBOid());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_ptr->getIBOid());

        /// Apparently, the below is buffer specific? It needs to be here at least. Look into VAO
        /// Or separate buffers for each attribute (corresponds better to the .obj 3d format)
        glVertexAttribPointer(attributes.vertex,       4, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));

        /// Draw call
        glDrawElements(GL_TRIANGLES, 3*mesh_ptr->getTriNum(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

        /// Not sure if this is necessary unless other code is badly written
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

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
