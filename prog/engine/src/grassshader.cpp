#include "grassshader.h"

GrassShader::GrassShader()
    //num_transforms(0)
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

    uniforms.worldpos_tex = glGetUniformLocation(getProgramID(), "worldpos_tex");
    glUniform1i(uniforms.worldpos_tex, 3);   // ALWAYS CHANNEL 3

    //uniforms.mv_mat = glGetUniformLocation(getProgramID(),"mv_mat") ;
    //uniforms.to_world_space_mat = glGetUniformLocation(getProgramID(), "to_world_space_mat");
    //uniforms.shadowmap_mvp_mat = glGetUniformLocation(getProgramID(), "shadowmap_mvp_mat");
    uniforms.cam_mat = glGetUniformLocation(getProgramID(),"cam_matrix") ;
    uniforms.light_bias_mat = glGetUniformLocation(getProgramID(),"light_bias_mat") ;

    uniforms.wind_params = glGetUniformLocation(getProgramID(),"wind_params") ;

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
    //mesh.fromFile("bush"); // never release this
    //tex.fromFile("grassing"); // never release this

    //sf::Vector2u tsize = image.getSize();
    //        std::cout<<"texture size = "<<size.x<<", "<<size.y<<"\n";
    //glTexImage2D(GL_TEXTURE_1D, 0, GL_RGBA, tsize.x, tsize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr() );
    //glGenerateMipmap(GL_TEXTURE_2D);    // Needed for texture filtering


    //world_transf = glm::mat4(1.0); // zero rotation at position zero
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

void GrassShader::extDraw(const SmallVisual& small_visual,
                          const Camera& cam_in,
                          const glm::mat4& light_mvp_mat)
{
    ShaderBase::switchTo();

    // All read from the same depth tex
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowmap_depth_texture);

    // Set view matrix and light matrix
    glm::mat4 view_mat = cam_in.getViewMatrix();
    glm::mat4 main_light_mvp_mat = light_mvp_mat; // This is used to calculate shadows (should go in the global buffer)

    glUniformMatrix4fv(uniforms.cam_mat, 1, false, &view_mat[0][0]);


    // Shadowmap related
    glm::mat4 biasMatrix(
                0.5, 0.0, 0.0, 0.0,
                0.0, 0.5, 0.0, 0.0,
                0.0, 0.0, 0.5, 0.0,
                0.5, 0.5, 0.5, 1.0
                );

    glm::mat4 light_bias_mat = biasMatrix * main_light_mvp_mat;
    glUniformMatrix4fv(uniforms.light_bias_mat, 1, false, &light_bias_mat[0][0]);

    glUniform4fv(uniforms.wind_params, 1, &(small_visual.wind_params[0]));

    std::shared_ptr<Mesh> mesh_ptr = small_visual.mesh;
    std::shared_ptr<Texture> tex_ptr = small_visual.tex;
    std::shared_ptr<Texture> tex2_ptr = small_visual.tex;

    // later make the material instancable
    Mesh::Material mat = mesh_ptr->getMaterial();

    // send mesh_ptr->specific uniforms to shader (materials)
    glUniform4fv(uniforms.ambient,   1,  &(mat.ambient[0])     ) ;
    glUniform4fv(uniforms.diffuse,   1,  &(mat.diffuse[0])     ) ;
    glUniform4fv(uniforms.specular,  1,  &(mat.specular[0])    ) ;
    glUniform1fv(uniforms.shininess, 1,  &(mat.shininess)      ) ;
    glUniform4fv(uniforms.emission,  1,  &(mat.emission[0])    ) ;

    tex_ptr->makeSureInVRAM(); // lazy loading to graphics card
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_ptr->getTBOid());

    tex2_ptr->makeSureInVRAM(); // lazy loading to graphics card
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex2_ptr->getTBOid());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_1D, small_visual.worldpos_tex_id);

    // Bind vertex data
    mesh_ptr->makeSureInVRAM(); // lazy loading to graphics card
    glBindBuffer(GL_ARRAY_BUFFER, mesh_ptr->getVBOid());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_ptr->getIBOid());


    glVertexAttribPointer(0,       4, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)                      );
    glVertexAttribPointer(1,       3, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(normalOffset)           );
    glVertexAttribPointer(2,    NUM_TEX_COORDS, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(texCoord0Offset)        );

    // Draw call
    //glDrawElementsInstanced(GL_TRIANGLES, 3*mesh_ptr->getTriNum(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0), small_visual.positions.size()); // <-----
    glDrawElementsInstanced(GL_TRIANGLES, 3*mesh_ptr->getTriNum(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0), small_visual.num_smvis); // <-----


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_1D, 0);
}
