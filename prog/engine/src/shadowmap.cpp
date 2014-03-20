#include "shadowmap.h"

ShadowMap::ShadowMap() :
    fbo(0),
    tex_depth(0),
    uniforms({0}),
    attributes({0}),
    light_vp_value(glm::mat4(1.0)),
    triggered(0)
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
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
    //uniforms.bone_mat = glGetUniformLocation(getProgramID(), "bone_mat");

    //Attributes
//    attributes.vertex = glGetAttribLocation(getProgramID(), "InVertex");
//    attributes.bone_index = glGetAttribLocation(getProgramID(), "bone_index");
//    attributes.bone_weight = glGetAttribLocation(getProgramID(), "bone_weight");



    // activate the attributes, why is this not needed here???
    //glEnableVertexAttribArray(attributes.vertex);
    //glEnableVertexAttribArray(attributes.bone_index);
    //glEnableVertexAttribArray(attributes.bone_weight);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    GLenum errCode;
    errCode = glGetError();
    if (errCode != GL_NO_ERROR) {
        std::cerr << "glGetError()) != GL_NO_ERROR-----SHADOW------\n";

        if (errCode == GL_INVALID_VALUE)
        {
            std::cerr << "GL_INVALID_VALUE\n";
        }
    }

    { // test
        GLfloat fbo_vertices[] =
        {
            -1, -1, 0.0, 1.0,
            1, -1, 0.0, 1.0,
            -1,  1, 0.0, 1.0,
            1,  1, 0.0, 1.0
        };
        glGenBuffers(1, &vbo_fbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

//        // get/enable attribs/uniforms and everything
//        char* attribute_name = "vertex";
//        attributes.vertex = glGetAttribLocation(getProgramID(), attribute_name);
//        if (attributes.vertex == -1)
//        {
//            fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
//    //        return 0;
//        }

        int isValid;
        glValidateProgram(getProgramID());
        glGetProgramiv(getProgramID(), GL_VALIDATE_STATUS, &isValid);

        if (!isValid)
        {
            std::cerr << "something wrong in shadowmap shader\n";
        }

        //glEnableVertexAttribArray(attributes.vertex);

    }


//    glFinish();

    std::cout << "shadowmap initialized\n";
}

void ShadowMap::activate(const glm::mat4 &light_vp)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0,0,1024,1024);

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
    glViewport(0,0,1024,1024);

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

    glUniformMatrix4fv(uniforms.bone_mat, num, false, &(actor->pose_matrices[0][0][0])); // <-- THIS!

    drawProp(actor);
}

void ShadowMap::drawProp(shared_ptr<Prop> prop)
{
//    { // OLD
//        for (auto rb_it = prop->render_batches.begin(); rb_it!= prop->render_batches.end(); rb_it++)
//        {
//    //        Mesh &mesh = *shared_ptr<Mesh>(*mesh_ptr_it); // deref the it to ptr, from ptr to mesh
//            shared_ptr<Mesh> mesh_ptr = shared_ptr<Mesh>(rb_it->mesh_ptr);
//            glm::mat4 transf_mat = rb_it->transf_mat;
//
//            // set the modelview matrix for this model
//            glm::mat4 tr = glm::translate(glm::mat4(1.0), prop->pos);
//            glm::mat4 rt = glm::mat4_cast(prop->rot);
//            glm::mat4 sc = glm::scale(glm::mat4(1.0), prop->scale);
//
//            glm::mat4 mv = light_vp_value;
//            glm::mat4 vertex_matrix  = mv * tr * rt * sc * transf_mat; // scale, then translate, then lookat.
//
//            glUniformMatrix4fv(uniforms.light_mvp_mat, 1, false, &vertex_matrix[0][0]);
//
//            // Bind vertex data
//            glBindBuffer(GL_ARRAY_BUFFER, mesh_ptr->getVBOid());
//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_ptr->getIBOid());
//
//            // Apparently, the below is buffer specific? It needs to be here at least. Look into VAO
//            // Or separate buffers for each attribute (corresponds better to the .obj 3d format)
//    //        glVertexAttribPointer(attributes.vertex,       4, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
//    //        glVertexAttribPointer(attributes.bone_index,   MAX_BONE_INFLUENCES, GL_INT,      GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_indexOffset)       );
//    //        glVertexAttribPointer(attributes.bone_weight,  MAX_BONE_INFLUENCES, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_weightOffset)      );
//
//            glVertexAttribPointer(0,       4, GL_FLOAT,    GL_FALSE,    sizeof(Vertex), BUFFER_OFFSET(0));
//            glVertexAttribPointer(1,  MAX_BONE_INFLUENCES, GL_INT,      GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_indexOffset)       );
//            glVertexAttribPointer(2,  MAX_BONE_INFLUENCES, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_weightOffset)      );
//            // glDraw checklist...
//            // All uniforms set?
//            // buffers work on other shader, so it should be fine
//
//    //        std::cout << "vbo_id: \n" << mesh_ptr->getVBOid() << "\n\n";
//    //        std::cout << "ibo_id: \n" << mesh_ptr->getIBOid() << "\n\n";
//    //
//    //        std::cout << "mesh_ptr->getTriNum(): \n" << mesh_ptr->getTriNum() << "\n\n";
//    //
//    //        std::cout << "glGetError(): " << ((glGetError()!=GL_NO_ERROR) ? "error" : "no error" ) << "\n";
//    //        std::cout << "calling glDrawElements\n";
//            // Draw call
//
//
////            if (triggered > 4000) // segfaults without this for some reason connected to the framebuffer...
////            {
////                std::cout << "drawing\n";
//                glDrawElements(GL_TRIANGLES, 3*mesh_ptr->getTriNum(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
////            }
////
////            else
////            {
////                triggered++;
////            }
//
//
//            //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
//
//            // Not sure if this is necessary unless other code is badly written
//            glBindBuffer(GL_ARRAY_BUFFER, 0);
//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//        }
//    } // OLD


    { // TEST
        //std::cout << "drawing arrays\n";
        glm::mat4 mv = light_vp_value;
        glm::mat4 vertex_matrix  = mv;// scale, then translate, then lookat.

        glUniformMatrix4fv(uniforms.light_mvp_mat, 1, false, &vertex_matrix[0][0]);

        // Bind vertex data
        glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
        glVertexAttribPointer(
            0,                  // attribute channel
            4,                  // number of elements per vertex, here (x,y,z,w)
            GL_FLOAT,           // the type of each element
            GL_FALSE,           // take our values as-is
            0,                  // no extra data between each position
            0                   // offset of first element
        );
//        if (triggered > 500)
//        {
//            std::cout << "drawing\n";
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//        }
//
//        else
//        {
//            triggered++;
//        }

        //std::cout << "drawing arrays\n";
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
