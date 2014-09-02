#include "grassshader.h"

GrassShader::GrassShader()
{
    //ctor
}

GrassShader::~GrassShader()

{
    //dtor
}

void GrassShader::init(GLuint shadowmap_depth_texture, GLuint global_uniforms_binding)
{
    debug = true;

    ShaderBase::load("shaders/ld_tinst_vert.glsl", "shaders/simple_frag.glsl");

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

    uniforms.to_world_space_mat = glGetUniformLocation(getProgramID(), "to_world_space_mat");
    uniforms.shadowmap_mvp_mat = glGetUniformLocation(getProgramID(), "shadowmap_mvp_mat");

    // global data
    // ASSIGN a block index to "GlobalUniforms" in Program
    //
    //       Binding0
    //             \
    //   Program    UBO
    //
    uniforms.globalUniformsBlockIndex = glGetUniformBlockIndex(getProgramID(), "GlobalUniforms");

//    // CONNECT the UBO to the Binding Index
//    //
//    //       Binding0
//    //       /     \                Finished!
//    //   Program    UBO
//    //
    glUniformBlockBinding(getProgramID(), uniforms.globalUniformsBlockIndex ,
                                          global_uniforms_binding);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    // Store the shadow map depth texture
    // internally. This will not change over time
    this->shadowmap_depth_texture = shadowmap_depth_texture;

    // initialize geometry
    //"quad", "grass1"
    mesh.fromFile("rabbit"); // never release this
    tex.fromFile("rabbit_d"); // never release this

    if (debug) std::cout << "bound vbo_id " << mesh.getVBOid() << "\n";
    if (debug) std::cout << "bound texture " << tex.getTBOid() << "\n";

    world_transf = glm::mat4(1.0); // zero rotation at position zero
}

void GrassShader::unload()
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

void GrassShader::updatePositions(int num, glm::vec4* positions)
{

}
//
//void GrassShader::activate(const Camera& cam_in, const glm::mat4& light_mvp_mat)
//{
//
//}

void GrassShader::draw(const Camera& cam_in, const glm::mat4& light_mvp_mat)
{
    //std::cout << "drawing grass \n";
// switch to main shader
    ShaderBase::switchTo();

    // All read from the same depth tex
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowmap_depth_texture);

    // Set view matrix and light matrix
    glm::mat4 view_mat = cam_in.getViewMatrix();
    glm::mat4 main_light_mvp_mat = light_mvp_mat; // This is used to calculate shadows (should go in the global buffer)

        // set the modelview matrix for this model
//        glm::mat4 tr = glm::translate(glm::mat4(1.0), prop->pos);
//        glm::mat4 rt = glm::mat4_cast(prop->rot);
//        glm::mat4 sc = glm::scale(glm::mat4(1.0), prop->scale);
//
        glm::mat4 obj_to_world_space_mat = world_transf;
//
//        if (debug) std::cout << "trans identity " << obj_to_world_space_mat[0][0] << "\n";
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

        glm::mat4 light_mvp_mat_refable  = biasMatrix * main_light_mvp_mat * obj_to_world_space_mat;
        glUniformMatrix4fv(uniforms.shadowmap_mvp_mat, 1, false, &light_mvp_mat_refable[0][0]);

    //} // finished instancing

    // send all the instanced uniforms
    // glUniformMatrix4fv(uniforms.mv_mat, N_INST, false, &vertex_matrix[0][0][0]); // mvp matrix
    // glUniformMatrix4fv(uniforms.to_world_space_mat, N_INST, false, &obj_to_world_space_mat[0][0][0]); // to world_space
    // glUniformMatrix4fv(uniforms.shadowmap_mvp_mat, N_INST, false, &light_mvp_mat_refable[0][0][0]); // shadowmap mvp


    // later make the material instancable
    Mesh::Material mat = mesh.getMaterial();

    // send mesh_ptr->specific uniforms to shader (materials)
    glUniform4fv(uniforms.ambient,   1,  &(mat.ambient[0])     ) ;
    glUniform4fv(uniforms.diffuse,   1,  &(mat.diffuse[0])     ) ;
    glUniform4fv(uniforms.specular,  1,  &(mat.specular[0])    ) ;
    glUniform1fv(uniforms.shininess, 1,  &(mat.shininess)      ) ;
    glUniform4fv(uniforms.emission,  1,  &(mat.emission[0])    ) ;

    // // glUniform4fv(uniforms.material_cols, N_INST*4, &mats.mat_cols[0][0][0]); // amb. diff. spec. emi. colors
    // // glUniform1fv(uniforms.shininess, N_INST, &mats.shiny[0][0][0]); // shininess value

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.getTBOid());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex.getTBOid());

    // Bind vertex data
    glBindBuffer(GL_ARRAY_BUFFER, mesh.getVBOid());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIBOid());

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
    //glVertexAttribPointer(3,   MAX_BONE_INFLUENCES, GL_INT,      GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_indexOffset)       );
    //glVertexAttribPointer(4,  MAX_BONE_INFLUENCES, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_weightOffset)      );

//    if (debug) std::cout << "numvertices " << 3*mesh.getTriNum() << "\n";
//    if (debug) std::cout << "bound vbo_id " << mesh.getVBOid() << "\n";
//    if (debug) std::cout << "bound texture " << tex.getTBOid() << "\n";

    // Draw call
    glDrawElementsInstanced(GL_TRIANGLES, 3*mesh.getTriNum(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0), 1); // <-----

    // Not sure if this is necessary unless other code is badly written
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}
