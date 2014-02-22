#include "shader.h"


Shader::Shader()
{
    /// default helper variables
    loaded = false;
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
    /// Init the shader program
    GLuint vertexshader = initshaders(GL_VERTEX_SHADER, vertex_shader.c_str()) ;
    GLuint fragmentshader = initshaders(GL_FRAGMENT_SHADER, fragment_shader.c_str()) ;
    program_id = initprogram(vertexshader, fragmentshader) ;

    /// set the uniform locations
    num_lights = glGetUniformLocation(program_id,"num_lights") ;
    light_posns = glGetUniformLocation(program_id,"light_posns") ;
    light_cols = glGetUniformLocation(program_id,"light_cols") ;

    ambient = glGetUniformLocation(program_id,"ambient") ;
    diffuse = glGetUniformLocation(program_id,"diffuse") ;
    specular = glGetUniformLocation(program_id,"specular") ;
    shininess = glGetUniformLocation(program_id,"shininess") ;
    emission = glGetUniformLocation(program_id,"emission") ;

//    hasTexture = glGetUniformLocation(program_id, "hasTexture") ;
    tex = glGetUniformLocation(program_id, "tex");
    glUniform1i(tex, 0);

//    time = glGetUniformLocation(program_id, "time");

    bone_mat = glGetUniformLocation(program_id, "bone_mat");
//    nobo_mat = glGetUniformLocation(program_id, "nobo_mat");
    mv_mat = glGetUniformLocation(program_id, "mv_mat");
    //nrm_mat = glGetUniformLocation(program_id, "nrm_mat");

    /// set the attribute locations
    vertex = glGetAttribLocation(program_id, "InVertex") ;
    normal = glGetAttribLocation(program_id, "InNormal") ;
    texCoord0 = glGetAttribLocation(program_id, "InTexCoord") ;
    bone_index = glGetAttribLocation(program_id, "bone_index") ;
    bone_weight = glGetAttribLocation(program_id, "bone_weight") ;

    /// activate the attributes
    glEnableVertexAttribArray(vertex);
    glEnableVertexAttribArray(normal);
    glEnableVertexAttribArray(texCoord0);
    glEnableVertexAttribArray(bone_index);
    glEnableVertexAttribArray(bone_weight);

    loaded = true;
}

void Shader::unload()
{
    /// deleting the shader object
    if (loaded)
    {
        /// not sure if this is needed
        glDisableVertexAttribArray(vertex);
        glDisableVertexAttribArray(normal);
        glDisableVertexAttribArray(texCoord0);
        glDisableVertexAttribArray(bone_index);
        glDisableVertexAttribArray(bone_weight);

        /// delete the program
        glDeleteProgram(program_id);

        loaded = false;
    }
}

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const unsigned short normalOffset = 1*sizeof(glm::vec4);
const unsigned short texCoord0Offset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3);
const unsigned short bone_indexOffset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3)+2*sizeof(float);
const unsigned short bone_weightOffset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3)+2*sizeof(float)+MAX_BONE_INFLUENCES*sizeof(int);


void Shader::switchTo()
{
    if (loaded)
    {
        glUseProgram(program_id);
    }
    else cerr << "Tried to use shader program before it was loaded\n";
}

void Shader::setLights(glm::mat4 mv)
{
    /// transform light to eye coordinates
    glm::vec4 light_pos_trans = mv * glm::vec4(1.0, 1.0, 1.0, 1.0);

    /// set light color
    glm::vec4 light_col = glm::vec4(1.0, 1.0, 1.0, 1.0);

    /// send uniforms
    glUniform1i(num_lights, 1) ;
    glUniform4fv(light_posns, 1, &(light_pos_trans[0]));
    glUniform4fv(light_cols,  1, &(light_col[0]));

}

void Shader::drawActor(shared_ptr<Actor> actor, glm::mat4 mv)
{
    /// Set bones
    int num = (actor->num_pose_matrices <= 50) ? actor->num_pose_matrices : 50;

//    /// dump bones to cout
//    for (int i_bone = 0; i_bone<actor->num_pose_matrices; i_bone++)
//    {
//        std::cout<<"bone matrix "<<i_bone<<"\n";
//        std::cout<<"";
//        for (int i = 0; i<4; i++)
//        {
//            std::cout<<"";
//            for (int j = 0; j<4; j++)
//            {
//                std::cout << actor->pose_matrices[i_bone][j][i];
//                if(j<3)std::cout<<",";
//            }
//            std::cout<<"";
//            if(i<3)std::cout<<"\n";
//        }
//        std::cout<<"\n\n";
//    }
//
//    system("PAUSE");

    glUniformMatrix4fv(bone_mat, num, true, &(actor->pose_matrices[0][0][0])); // <-- THIS!

    drawProp(actor, mv);
}

void Shader::drawProp(shared_ptr<Prop> prop, glm::mat4 mv)
{
    /// This is very dirty... TODO: Get transformation info from prop,
    /// and the rest from each mesh. Put things into private and make getter
    /// functions! Could make custom lambda-iterators as well, returning the info for each
    /// mesh instead of each mesh

//    for (auto mesh_ptr_it = prop->mesh_ptrs.begin(); mesh_ptr_it!= prop->mesh_ptrs.end(); mesh_ptr_it++)
    for (auto rb_it = prop->render_batches.begin(); rb_it!= prop->render_batches.end(); rb_it++)
    {
//        Mesh &mesh = *shared_ptr<Mesh>(*mesh_ptr_it); /// deref the it to ptr, from ptr to mesh
        shared_ptr<Mesh> mesh_ptr = shared_ptr<Mesh>(rb_it->mesh_ptr);
        shared_ptr<Texture> tex_ptr = shared_ptr<Texture>(rb_it->tex_ptr);
        glm::mat4 transf_mat = rb_it->transf_mat;
        /// send some clean bone matrices
        //    mat4 clearMatrix = mat4(1.0);
        //    glUniformMatrix4fv(bone_mat, 1, true, &clearMatrix[0][0]); // <-- THIS!

        //    int a;
        /// set the modelview matrix for this model
        glm::mat4 tr = glm::translate(glm::mat4(1.0), prop->pos);
        glm::mat4 rt = glm::mat4_cast(prop->rot);
        glm::mat4 sc = glm::scale(glm::mat4(1.0), prop->scale);

//        glm::mat4 vertex_matrix  = mv * tr * rt * sc;
        /// For bone debugging
        glm::mat4 vertex_matrix  = mv * tr * rt * sc * transf_mat; // scale, then translate, then lookat.

        glUniformMatrix4fv(mv_mat, 1, false, &vertex_matrix[0][0]);

//        /// normal matrix
//        glm::mat4 normal_matrix = glm::inverse(glm::transpose(vertex_matrix));
//        glUniformMatrix4fv(nrm_mat, 1, false, &normal_matrix[0][0]);
        // mm::matPrint(normal_matrix);

        //    // debug by printing all info being sent to shader:
        //    cout<<"numbones = "<<skeleton.numBones<<"\n";
        //    for (int i = 0; i<skeleton.numBones; i++) {
        //        mm::matPrint(skeleton.bone_matrices[i]);
        //    }
        //    // looks good
        // /// send bone specific info
        // glUniformMatrix4fv(shader->bone_matLoc, skeleton.numBones, true, &skeleton.bone_matrices[0][0][0]); // <-- THIS!
        // glUniformMatrix4fv(shader->nobo_matLoc, skeleton.numBones, true, &skeleton.norm_matrices[0][0][0]);


        Mesh::Material mat = mesh_ptr->getMaterial();

        /// send mesh_ptr->specific uniforms to shader (materials)
        glUniform4fv(ambient,   1,  &(mat.ambient[0])     ) ;
        glUniform4fv(diffuse,   1,  &(mat.diffuse[0])     ) ;
        glUniform4fv(specular,  1,  &(mat.specular[0])    ) ;
        glUniform1fv(shininess, 1,  &(mat.shininess)      ) ;
        glUniform4fv(emission,  1,  &(mat.emission[0])    ) ;

        //    glUniform1i(hasTexture, mat.hasTexture);


        //    if (hasTexture)
        //    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_ptr->getTBOid());
//                cout<<"binding texture "<<tex_ptr->getTBOid()<<" to  channel 0"<<"\n";
        //    }

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


/// HELPER FUNCTIONS (DO NOT USUALLY NEED TO BE EDITED

string Shader::textFileRead (const char * filename)
{
    string str, ret = "" ;
    ifstream in ;
    in.open(filename) ;
    if (in.is_open())
    {
        getline (in, str) ;
        while (in)
        {
            ret += str + "\n" ;
            getline (in, str) ;
        }
        //    cout << "Shader below\n" << ret << "\n" ;
        return ret ;
    }
    else
    {
        cerr << "Unable to Open File " << filename << "\n" ;
        throw 2 ;
    }
}

void Shader::programerrors (const GLint program)
{
    GLint length ;
    GLchar * log ;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length) ;
    log = new GLchar[length+1] ;
    glGetProgramInfoLog(program, length, &length, log) ;
    cerr << "Compile Error, Log Below\n" << log << "\n" ;
    delete [] log ;
}
void Shader::shadererrors (const GLint shader)
{
    GLint length ;
    GLchar * log ;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length) ;
    log = new GLchar[length+1] ;
    glGetShaderInfoLog(shader, length, &length, log) ;
    cerr << "Compile Error, Log Below\n" << log << "\n" ;
    delete [] log ;
}

GLuint Shader::initshaders (GLenum type, const char *filename)
{
    GLuint shader = glCreateShader(type) ;
    GLint compiled ;
    string str = textFileRead (filename) ;
    GLchar * cstr = new GLchar[str.size()+1] ;
    const GLchar * cstr2 = cstr ; // Weirdness to get a const char
    strcpy(cstr,str.c_str()) ;
    glShaderSource (shader, 1, &cstr2, NULL) ;
    glCompileShader (shader) ;
    glGetShaderiv (shader, GL_COMPILE_STATUS, &compiled) ;
    if (!compiled)
    {
        shadererrors (shader) ;
        throw 3 ;
    }
    delete [] cstr;
//    delete [] cstr2;
    return shader ;
}

GLuint Shader::initprogram (GLuint vertexshader, GLuint fragmentshader)
{
    GLuint program = glCreateProgram() ;
    GLint linked ;
    glAttachShader(program, vertexshader) ;
    glAttachShader(program, fragmentshader) ;
    glLinkProgram(program) ;
    glGetProgramiv(program, GL_LINK_STATUS, &linked) ;
    if (linked) glUseProgram(program) ;
    else
    {
        programerrors(program) ;
        throw 4 ;
    }

    /// When the program is created, the shaders are no longer needed
    glDetachShader(program, vertexshader);
    glDetachShader(program, fragmentshader);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);

    return program ;
}

