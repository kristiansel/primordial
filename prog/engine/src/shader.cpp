#include "shader.h"


Shader::Shader()
{

}

Shader::Shader(string vertex_shader, string fragment_shader)
{
    load(vertex_shader, fragment_shader);
}

Shader::~Shader()
{
    unload();
}

void Shader::load(string vertex_shader, string fragment_shader)
{
    ShaderBase::load(vertex_shader, fragment_shader);

    /// set the uniform locations
    num_lights = glGetUniformLocation(getProgramID(),"num_lights") ;
    light_posns = glGetUniformLocation(getProgramID(),"light_posns") ;
    light_cols = glGetUniformLocation(getProgramID(),"light_cols") ;

    ambient = glGetUniformLocation(getProgramID(),"ambient") ;
    diffuse = glGetUniformLocation(getProgramID(),"diffuse") ;
    specular = glGetUniformLocation(getProgramID(),"specular") ;
    shininess = glGetUniformLocation(getProgramID(),"shininess") ;
    emission = glGetUniformLocation(getProgramID(),"emission") ;

    tex = glGetUniformLocation(getProgramID(), "tex");
    glUniform1i(tex, 0);            /// ALWAYS CHANNEL 0

    shadow_depth = glGetUniformLocation(getProgramID(), "shadow_depth");
    glUniform1i(shadow_depth, 1);   /// ALWAYS CHANNEL 1

    bone_mat = glGetUniformLocation(getProgramID(), "bone_mat");

    mv_mat = glGetUniformLocation(getProgramID(), "mv_mat");

    shadowmap_mvp_mat = glGetUniformLocation(getProgramID(), "shadowmap_mvp_mat");

    /// set the attribute locations
    vertex = glGetAttribLocation(getProgramID(), "InVertex") ;
    normal = glGetAttribLocation(getProgramID(), "InNormal") ;
    texCoord0 = glGetAttribLocation(getProgramID(), "InTexCoord") ;
    bone_index = glGetAttribLocation(getProgramID(), "bone_index") ;
    bone_weight = glGetAttribLocation(getProgramID(), "bone_weight") ;

    /// activate the attributes
    glEnableVertexAttribArray(vertex);
    glEnableVertexAttribArray(normal);
    glEnableVertexAttribArray(texCoord0);
    glEnableVertexAttribArray(bone_index);
    glEnableVertexAttribArray(bone_weight);
}

void Shader::unload()
{
    /// deleting the shader object
    if (isLoaded())
    {
        /// not sure if this is needed
        glDisableVertexAttribArray(vertex);
        glDisableVertexAttribArray(normal);
        glDisableVertexAttribArray(texCoord0);
        glDisableVertexAttribArray(bone_index);
        glDisableVertexAttribArray(bone_weight);
    }

    ShaderBase::unload();
}


void Shader::activate(glm::mat4 mv, glm::mat4 light_mvp_mat, GLuint shadow_depth)
{
    /// switch to main shader
    ShaderBase::switchTo();

    /// clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// transform light to eye coordinates
    glm::vec4 light_pos_trans = mv * glm::vec4(1.0, 1.0, 1.0, 0.0);

    /// set light color
    glm::vec4 light_col = glm::vec4(1.0, 1.0, 1.0, 1.0);

    /// send uniforms
    glUniform1i(num_lights, 1) ;
    glUniform4fv(light_posns, 1, &(light_pos_trans[0]));
    glUniform4fv(light_cols,  1, &(light_col[0]));

    /// All read from the same depth tex
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadow_depth);

    /// Change this
    this->light_mvp_mat =  light_mvp_mat; /// Store for reuse when drawing each..
}

void Shader::drawActor(shared_ptr<Actor> actor, glm::mat4 mv)
{
    /// Set bones
    int num = (actor->num_pose_matrices <= MAX_BONE_NUM) ? actor->num_pose_matrices : MAX_BONE_NUM;

    glUniformMatrix4fv(bone_mat, num, true, &(actor->pose_matrices[0][0][0])); // <-- THIS!

    drawProp(actor, mv);
}

void Shader::drawProp(shared_ptr<Prop> prop, glm::mat4 mv)
{
    for (auto rb_it = prop->render_batches.begin(); rb_it!= prop->render_batches.end(); rb_it++)
    {
        shared_ptr<Mesh> mesh_ptr = shared_ptr<Mesh>(rb_it->mesh_ptr);
        shared_ptr<Texture> tex_ptr = shared_ptr<Texture>(rb_it->tex_ptr);
        glm::mat4 transf_mat = rb_it->transf_mat;

        /// set the modelview matrix for this model
        glm::mat4 tr = glm::translate(glm::mat4(1.0), prop->pos);
        glm::mat4 rt = glm::mat4_cast(prop->rot);
        glm::mat4 sc = glm::scale(glm::mat4(1.0), prop->scale);

        glm::mat4 vertex_matrix  = mv * tr * rt * sc * transf_mat; // scale, then translate, then lookat.

        glUniformMatrix4fv(mv_mat, 1, false, &vertex_matrix[0][0]);



        /// Shadowmap related
        glm::mat4 biasMatrix(
                    0.5, 0.0, 0.0, 0.0,
                    0.0, 0.5, 0.0, 0.0,
                    0.0, 0.0, 0.5, 0.0,
                    0.5, 0.5, 0.5, 1.0
                    );

        glm::mat4 light_mvp_mat_refable  = biasMatrix * light_mvp_mat * tr * rt * sc * transf_mat;
        glUniformMatrix4fv(shadowmap_mvp_mat, 1, false, &light_mvp_mat_refable[0][0]);

        Mesh::Material mat = mesh_ptr->getMaterial();

        /// send mesh_ptr->specific uniforms to shader (materials)
        glUniform4fv(ambient,   1,  &(mat.ambient[0])     ) ;
        glUniform4fv(diffuse,   1,  &(mat.diffuse[0])     ) ;
        glUniform4fv(specular,  1,  &(mat.specular[0])    ) ;
        glUniform1fv(shininess, 1,  &(mat.shininess)      ) ;
        glUniform4fv(emission,  1,  &(mat.emission[0])    ) ;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_ptr->getTBOid());

        /// Bind vertex data
        glBindBuffer(GL_ARRAY_BUFFER, mesh_ptr->getVBOid());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_ptr->getIBOid());

        /// Apparently, the below is buffer specific? It needs to be here at least. Look into VAO
        /// Or separate buffers for each attribute (corresponds better to the .obj 3d format)
        glVertexAttribPointer(vertex,       4, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)                      );
        glVertexAttribPointer(normal,       3, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(normalOffset)           );
        glVertexAttribPointer(texCoord0,    2, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(texCoord0Offset)        );
        glVertexAttribPointer(bone_index,   MAX_BONE_INFLUENCES, GL_INT,      GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_indexOffset)       );
        glVertexAttribPointer(bone_weight,  MAX_BONE_INFLUENCES, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(bone_weightOffset)      );

        /// Draw call
        glDrawElements(GL_TRIANGLES, 3*mesh_ptr->getTriNum(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

        /// Not sure if this is necessary unless other code is badly written
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
