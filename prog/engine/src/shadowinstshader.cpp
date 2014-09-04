#include "shadowinstshader.h"

ShadowInstShader::ShadowInstShader()
{
    //ctor
}

ShadowInstShader::~ShadowInstShader()
{
    //dtor
}


void ShadowInstShader::init()
{
    ShaderBase::load("shaders/shadow_instsmall_vert.glsl", "shaders/shadow_map_frag.glsl");

    uniforms.worldpos_tex = glGetUniformLocation(getProgramID(),"worldpos_tex") ;
    glUniform1i(uniforms.worldpos_tex, 3);   // ALWAYS CHANNEL 3

    uniforms.light_mvp = glGetUniformLocation(getProgramID(),"light_mvp") ;

    uniforms.tex = glGetUniformLocation(getProgramID(),"tex") ;
    glUniform1i(uniforms.tex, 0);            // ALWAYS CHANNEL 0

    // do I really need this in all shaders? maybe just once in the renderer???
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);



}

void ShadowInstShader::draw(SmallVisual &small_visual, glm::mat4 &light_vp)
{
    // switch shader program to this
    ShaderBase::switchTo();

    glUniformMatrix4fv(uniforms.light_mvp, 1, false, &(light_vp[0][0]));

    std::shared_ptr<Mesh> mesh_ptr = small_visual.mesh;
    std::shared_ptr<Texture> tex_ptr = small_visual.tex;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_ptr->getTBOid());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_1D, small_visual.worldpos_tex_id);

    // Bind vertex data
    glBindBuffer(GL_ARRAY_BUFFER, mesh_ptr->getVBOid());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_ptr->getIBOid());


    glVertexAttribPointer(0,       4, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)                      );
    glVertexAttribPointer(2,    NUM_TEX_COORDS, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(texCoord0Offset)        );

    // Draw call
    glDrawElementsInstanced(GL_TRIANGLES, 3*mesh_ptr->getTriNum(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0), small_visual.positions.size()); // <-----

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_1D, 0);

}
