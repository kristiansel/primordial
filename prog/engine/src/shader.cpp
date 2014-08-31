#include "shader.h"


Shader::Shader()
{
    for (int i_cm = 0; i_cm<MAX_BONE_NUM; i_cm++)
    {
        clear_matrices[i_cm] = glm::mat4(1.0);
    }
}

Shader::Shader(string vertex_shader, string fragment_shader)
{
    load(vertex_shader, fragment_shader);
}

Shader::~Shader()
{
    unload();
}

void Shader::init(GLuint shadowmap_depth_texture)
{
    ShaderBase::load("shaders/simple_vert.glsl", "shaders/simple_frag.glsl");

    // set the uniform locations
//    uniforms.num_lights = glGetUniformLocation(getProgramID(),"num_lights") ;
//    uniforms.light_posns = glGetUniformLocation(getProgramID(),"light_posns") ;
//    uniforms.light_cols = glGetUniformLocation(getProgramID(),"light_cols") ;
    uniforms.main_light_dir = glGetUniformLocation(getProgramID(), "main_light_dir");
    uniforms.main_light_color = glGetUniformLocation(getProgramID(), "main_light_color");

    uniforms.ambient = glGetUniformLocation(getProgramID(),"ambient") ;
    uniforms.diffuse = glGetUniformLocation(getProgramID(),"diffuse") ;
    uniforms.specular = glGetUniformLocation(getProgramID(),"specular") ;
    uniforms.shininess = glGetUniformLocation(getProgramID(),"shininess") ;
    uniforms.emission = glGetUniformLocation(getProgramID(),"emission") ;

    uniforms.tex = glGetUniformLocation(getProgramID(), "tex");
    glUniform1i(uniforms.tex, 0);            // ALWAYS CHANNEL 0

    uniforms.shadow_depth = glGetUniformLocation(getProgramID(), "shadow_depth");
    glUniform1i(uniforms.shadow_depth, 1);   // ALWAYS CHANNEL 1

    uniforms.tex2 = glGetUniformLocation(getProgramID(), "tex2");
    glUniform1i(uniforms.tex2, 2);   // ALWAYS CHANNEL 2

    uniforms.bone_mat = glGetUniformLocation(getProgramID(), "bone_mat");
    uniforms.mv_mat = glGetUniformLocation(getProgramID(), "mv_mat");
    uniforms.proj_mat = glGetUniformLocation(getProgramID(), "proj_mat");
    uniforms.to_world_space_mat = glGetUniformLocation(getProgramID(), "to_world_space_mat");
    uniforms.shadowmap_mvp_mat = glGetUniformLocation(getProgramID(), "shadowmap_mvp_mat");

    uniforms.fog_color = glGetUniformLocation(getProgramID(), "fog_color");
    uniforms.sky_color = glGetUniformLocation(getProgramID(), "sky_color");
    uniforms.zfar = glGetUniformLocation(getProgramID(), "zfar");

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    // Store the shadow map depth texture
    // internally. This will not change over time
    this->shadowmap_depth_texture = shadowmap_depth_texture;
}


void Shader::unload()
{
    // deleting the shader object
    if (isLoaded())
    {
        // not sure if this is needed
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);
    }

    ShaderBase::unload();
}

void Shader::activate(const Camera &cam_in,
                      const glm::vec4 fog_color,
                      const glm::vec4 sky_color,
                      const glm::mat4 &light_mvp_mat,
                      const DirLight &main_light)
{
    // switch to main shader
    ShaderBase::switchTo();

    // clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // All read from the same depth tex
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowmap_depth_texture);

    // Send projection matrix:
    glm::mat4 p = cam_in.getProjectionMatrix();
    glUniformMatrix4fv(uniforms.proj_mat, 1, false, &p[0][0] );

//    std::cout << "cam_in.getProjectionMatrix():  \n" << p << "\n\n";
//
//    std::terminate();

    // Set view matrix and light matrix
    view_mat = cam_in.getViewMatrix();
    main_light_mvp_mat = light_mvp_mat;

    // Update the light
    main_light_dir = view_mat * glm::vec4(main_light.dir, 0.0);
    main_light_color = main_light.color;

    // Send light uniforms
    glUniform3fv(uniforms.main_light_dir, 1, &(this->main_light_dir[0]));
    glUniform4fv(uniforms.main_light_color,  1, &(this->main_light_color[0]));

    // Fog uniforms
    glUniform4fv(uniforms.fog_color, 1, &fog_color[0]);
    glUniform4fv(uniforms.sky_color, 1, &sky_color[0]);
    glUniform1f(uniforms.zfar, cam_in.farz);

//    // transform light to eye coordinates
//    glm::vec4 light_pos_trans = mv * glm::vec4(1.0, 1.0, 1.0, 0.0);
//
//    // set light color
//    glm::vec4 light_col = glm::vec4(1.0, 1.0, 1.0, 1.0);
//
//    // send uniforms
//    glUniform1i(uniforms.num_lights, 1) ;
//    glUniform4fv(uniforms.light_posns, 1, &(light_pos_trans[0]));
//    glUniform4fv(uniforms.light_cols,  1, &(light_col[0]));


}

void Shader::clearBoneMatrices()
{
    glUniformMatrix4fv(uniforms.bone_mat, MAX_BONE_NUM, true, &clear_matrices[0][0][0]); // <-- THIS!
}

void Shader::drawActor(shared_ptr<Actor> actor)
{
    // Set bones
    int num = (actor->num_pose_matrices <= MAX_BONE_NUM) ? actor->num_pose_matrices : MAX_BONE_NUM;

//    for (int i = 0; i<actor->num_pose_matrices; i++)
//    {
//        glm::mat4 &matrix = actor->pose_matrices[i];
//        matrix = glm::transpose(matrix);
//    }

    glUniformMatrix4fv(uniforms.bone_mat, num, false, &(actor->pose_matrices[0][0][0])); // <-- THIS!

    drawProp(actor);
}

void Shader::drawProp(shared_ptr<Prop> prop, bool debug)
{
    for (auto rb_it = prop->render_batches.begin(); rb_it!= prop->render_batches.end(); rb_it++)
    {
        shared_ptr<Mesh> mesh_ptr = shared_ptr<Mesh>(rb_it->mesh_ptr);
        shared_ptr<Texture> tex_ptr = shared_ptr<Texture>(rb_it->tex_ptr);
        shared_ptr<Texture> tex2_ptr = shared_ptr<Texture>(rb_it->tex2_ptr);


        //if (rb_it->m_instanced==false)
        { // everything form here on will be instanced
            glm::mat4 transf_mat = rb_it->transf_mat;

            if (debug) std::cout << "transfmat identity " << transf_mat[0][0]  << "\n";


            // set the modelview matrix for this model
            glm::mat4 tr = glm::translate(glm::mat4(1.0), prop->pos);
            glm::mat4 rt = glm::mat4_cast(prop->rot);
            glm::mat4 sc = glm::scale(glm::mat4(1.0), prop->scale);

            glm::mat4 obj_to_world_space_mat = tr * rt * sc * transf_mat;

            if (debug) std::cout << "trans identity " << obj_to_world_space_mat[0][0] << "\n";
            glm::mat4 vertex_matrix  = view_mat * obj_to_world_space_mat; // scale, then translate, then lookat.

            glUniformMatrix4fv(uniforms.mv_mat, 1, false, &vertex_matrix[0][0]);
            glUniformMatrix4fv(uniforms.to_world_space_mat, 1, false, &obj_to_world_space_mat[0][0]);



            // Shadowmap related
            glm::mat4 biasMatrix(
                        0.5, 0.0, 0.0, 0.0,
                        0.0, 0.5, 0.0, 0.0,
                        0.0, 0.0, 0.5, 0.0,
                        0.5, 0.5, 0.5, 1.0
                        );

            glm::mat4 light_mvp_mat_refable  = biasMatrix * main_light_mvp_mat * tr * rt * sc * transf_mat;
            glUniformMatrix4fv(uniforms.shadowmap_mvp_mat, 1, false, &light_mvp_mat_refable[0][0]);

        } // finished instancing

        // send all the instanced uniforms
        // glUniformMatrix4fv(uniforms.mv_mat, N_INST, false, &vertex_matrix[0][0][0]); // mvp matrix
        // glUniformMatrix4fv(uniforms.to_world_space_mat, N_INST, false, &obj_to_world_space_mat[0][0][0]); // to world_space
        // glUniformMatrix4fv(uniforms.shadowmap_mvp_mat, N_INST, false, &light_mvp_mat_refable[0][0][0]); // shadowmap mvp


        // later make the material instancable
        Mesh::Material mat = mesh_ptr->getMaterial();

        // send mesh_ptr->specific uniforms to shader (materials)
        glUniform4fv(uniforms.ambient,   1,  &(mat.ambient[0])     ) ;
        glUniform4fv(uniforms.diffuse,   1,  &(mat.diffuse[0])     ) ;
        glUniform4fv(uniforms.specular,  1,  &(mat.specular[0])    ) ;
        glUniform1fv(uniforms.shininess, 1,  &(mat.shininess)      ) ;
        glUniform4fv(uniforms.emission,  1,  &(mat.emission[0])    ) ;

        // // glUniform4fv(uniforms.material_cols, N_INST*4, &mats.mat_cols[0][0][0]); // amb. diff. spec. emi. colors
        // // glUniform1fv(uniforms.shininess, N_INST, &mats.shiny[0][0][0]); // shininess value

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_ptr->getTBOid());

        glActiveTexture(GL_TEXTURE2);
        if (rb_it->num_textures > 1) glBindTexture(GL_TEXTURE_2D, tex2_ptr->getTBOid());

        // Bind vertex data
        glBindBuffer(GL_ARRAY_BUFFER, mesh_ptr->getVBOid());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_ptr->getIBOid());

        // Apparently, the below is buffer specific? It needs to be here at least. Look into VAO
        // Or separate buffers for each attribute (corresponds better to the .obj 3d format)
//        glVertexAttribPointer(attributes.vertex,       4, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)                      );
//        glVertexAttribPointer(attributes.normal,       3, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(normalOffset)           );
//        glVertexAttribPointer(attributes.texCoord0,    2, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(texCoord0Offset)        );
//        glVertexAttribPointer(attributes.bone_index,   MAX_BONE_INFLUENCES, GL_INT,      GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_indexOffset)       );
//        glVertexAttribPointer(attributes.bone_weight,  MAX_BONE_INFLUENCES, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_weightOffset)      );


        glVertexAttribPointer(0,       4, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)                      );
        glVertexAttribPointer(1,       3, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(normalOffset)           );
        glVertexAttribPointer(2,    NUM_TEX_COORDS, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(texCoord0Offset)        );
        glVertexAttribPointer(3,   MAX_BONE_INFLUENCES, GL_INT,      GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_indexOffset)       );
        glVertexAttribPointer(4,  MAX_BONE_INFLUENCES, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_weightOffset)      );

        if (debug) std::cout << "numvertices " << 3*mesh_ptr->getTriNum() << "\n";
        if (debug) std::cout << "bound vbo_id " << mesh_ptr->getVBOid() << "\n";
        if (debug) std::cout << "bound texture " << tex_ptr->getTBOid() << "\n";

        // Draw call
        glDrawElementsInstanced(GL_TRIANGLES, 3*mesh_ptr->getTriNum(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0), 1); // <-----

        // Not sure if this is necessary unless other code is badly written
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
