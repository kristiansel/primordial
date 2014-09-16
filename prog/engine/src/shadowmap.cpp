#include "shadowmap.h"

ShadowMap::ShadowMap() :
    fbo(0),
    tex_depth(0),
    uniforms({0}),
//    attributes({0}),
    light_vp_value(glm::mat4(1.0)),
    triggered(0),
    resolution(1024) // 1024
{
    for (int i_cm = 0; i_cm<MAX_BONE_NUM; i_cm++)
    {
        clear_matrices[i_cm] = glm::mat4(1.0);
    }
}

void ShadowMap::init()
{
    fbo = 0;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &tex_depth);
    glBindTexture(GL_TEXTURE_2D, tex_depth);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float bord_color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bord_color);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    // glFramebufferTexture requires OpenGL 3.2
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex_depth, 0);

    glDrawBuffer(GL_NONE);

    glReadBuffer(GL_NONE); // Why is this needed?

    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "glCheckFramebufferStatus original image: error %p", status);
        std::cout<<"\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Create and compile our GLSL program from the shaders
    ShaderBase::load("shaders/shadow_map_vert.glsl", "shaders/shadow_map_frag.glsl" );

    //Uniforms
    uniforms.light_mvp_mat  = glGetUniformLocation(getProgramID(), "depthMVP");
    uniforms.bone_mat = glGetUniformLocation(getProgramID(), "bone_mat");
    uniforms.tex = glGetUniformLocation(getProgramID(), "tex");
    glUniform1i(uniforms.tex, 0);            // ALWAYS CHANNEL 0

    //Attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);


    GLenum errCode;
    errCode = glGetError();
    if (errCode != GL_NO_ERROR) {
        std::cerr << "glGetError()) != GL_NO_ERROR-----SHADOW------\n";

        if (errCode == GL_INVALID_VALUE)
        {
            std::cerr << "GL_INVALID_VALUE\n";
        }
    }

    std::cout << "shadowmap initialized\n";
}

void ShadowMap::activate(const glm::mat4 &light_vp)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0,0,resolution,resolution);

    glClearColor(0.f, 0.f, 0.f, 0.f);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // switch shader program to this
    ShaderBase::switchTo();

    // update the light view matrix
    light_vp_value = light_vp;
}

void ShadowMap::activateDrawContent(const glm::mat4 &light_vp)
{
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0,0,resolution,resolution);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // switch shader program to this
    ShaderBase::switchTo();

    // update the light view matrix
    light_vp_value = light_vp;
}

void ShadowMap::clearBoneMatrices()
{
    glUniformMatrix4fv(uniforms.bone_mat, MAX_BONE_NUM, false, &clear_matrices[0][0][0]); // <-- THIS!
}

void ShadowMap::deactivate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::drawActor(shared_ptr<Actor> actor)
{
    // Set bones
    int num = (actor->num_pose_matrices <= MAX_BONE_NUM) ? actor->num_pose_matrices : MAX_BONE_NUM;

    //glm::mat4 pose_matrices[MAX_BONE_NUM];

    //actor->poseMatrices(pose_matrices);



    glUniformMatrix4fv(uniforms.bone_mat, num, false, &(actor->pose_matrices[0][0][0])); // <-- THIS!

    //glUniformMatrix4fv(uniforms.bone_mat, num, false, &(pose_matrices[0][0][0]));

    drawProp(actor);
}

void ShadowMap::drawProp(shared_ptr<Prop> prop)
{
    for (auto rb_it = prop->render_batches.begin(); rb_it!= prop->render_batches.end(); rb_it++)
    {
        shared_ptr<Mesh> mesh_ptr = shared_ptr<Mesh>(rb_it->mesh_ptr);
        shared_ptr<Texture> tex_ptr = shared_ptr<Texture>(rb_it->tex_ptr);
        glm::mat4 transf_mat = rb_it->transf_mat;

        // set the modelview matrix for this model
        glm::mat4 tr = glm::translate(glm::mat4(1.0), prop->pos);
        glm::mat4 rt = glm::mat4_cast(prop->rot);
        glm::mat4 sc = glm::scale(glm::mat4(1.0), prop->scale);

        glm::mat4 mv = light_vp_value;
        glm::mat4 vertex_matrix  = mv * tr * rt * sc * transf_mat; // scale, then translate, then lookat.

        glUniformMatrix4fv(uniforms.light_mvp_mat, 1, false, &vertex_matrix[0][0]);

        // activate texture
        tex_ptr->makeSureInVRAM(); // lazy loading to graphics card
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_ptr->getTBOid());

        // Bind vertex data
        mesh_ptr->makeSureInVRAM(); // lazy loading to graphics card
        glBindBuffer(GL_ARRAY_BUFFER, mesh_ptr->getVBOid());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_ptr->getIBOid());


        // Apparently segfault is encountered if attempting to draw without setting all enabled attrib pointers...
        glVertexAttribPointer(0,       4, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)                      );
        glVertexAttribPointer(1,       3, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(normalOffset)           );
        glVertexAttribPointer(2,    2, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(texCoord0Offset)        );
        glVertexAttribPointer(3,   MAX_BONE_INFLUENCES, GL_INT,      GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_indexOffset)       );
        glVertexAttribPointer(4,   MAX_BONE_INFLUENCES, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_weightOffset)      );

        glDrawElements(GL_TRIANGLES, 3*mesh_ptr->getTriNum(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

        // Not sure if this is necessary unless other code is badly written
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
