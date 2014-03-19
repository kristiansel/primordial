#include "combinationstage.h"


CombinationStage::CombinationStage()
{
//    isLoaded() = false;
};

CombinationStage::~CombinationStage()
{
    if (isLoaded())
    {
        cout<<"deleting combination stage buffers\n";
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteTextures(1, &fbo_texture);
        glDeleteFramebuffers(1, &fbo);
        glDeleteRenderbuffers(1, &rbo_depth);
        glDisableVertexAttribArray(attribute_v_coord_postproc);
        glDeleteBuffers(1, &vbo_fbo_vertices);
    }
    ShaderBase::unload();
};

//void CombinationStage::init(int w, int h, char* shader_path)
void CombinationStage::init(int w, int h)
{

    //TEXTURE
    //glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &fbo_texture);
//    cout<<"comb_fbo_texture id: "<<fbo_texture<<"\n";
    glBindTexture(GL_TEXTURE_2D, fbo_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    //DEPTH BUFFER (RENDER)
    glGenRenderbuffers(1, &rbo_depth);
//    cout<<"comb_rbo_depth id: "<<rbo_depth<<"\n";
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    //FRAMEBUFFER2
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);

        //Depth Renderbuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
        GLenum status;
        if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
        {
            fprintf(stderr, "glCheckFramebufferStatus original image: error %p", status);
            std::cout<<"\n";
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    GLfloat fbo_vertices[] =
    {
        -1, -1,
        1, -1,
        -1,  1,
        1,  1,
    };
    glGenBuffers(1, &vbo_fbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    }
    ShaderBase::load("shaders/pp_wave.vert.glsl", "shaders/pp_comb.frag.glsl");

    // get/enable attribs/uniforms and everything
    char* attribute_name = "v_coord";
    attribute_v_coord_postproc = glGetAttribLocation(getProgramID(), attribute_name);
    if (attribute_v_coord_postproc == -1)
    {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
//        return 0;
    }
    uniform_textures[0] = glGetUniformLocation(getProgramID(), "texture0");
    uniform_textures[1] = glGetUniformLocation(getProgramID(), "texture1");
    uniform_w = glGetUniformLocation(getProgramID(), "weights");

    int isValid;
    glValidateProgram(getProgramID());
    glGetProgramiv(getProgramID(), GL_VALIDATE_STATUS, &isValid);

//      if (uniform_fbo_texture == -1) {
//        fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
//       // return 0;
//      }

    glEnableVertexAttribArray(attribute_v_coord_postproc);

//    isLoaded() = true;
};

void CombinationStage::activate(float w0, float w1)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    ShaderBase::switchTo();

    //glUseProgram(getProgramID());

    float weights[2];
    weights[0]=w0;
    weights[1]=w1;

    glUniform1fv(uniform_w, 2, &weights[0]);
}
void CombinationStage::activateTextures(GLuint color0, GLuint color1)
{
    glUniform1i(uniform_textures[0], 0); // set uniform texture to channel 0
    glUniform1i(uniform_textures[1], 1); // set uniform depth to channel 1

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, color1);
};
void CombinationStage::draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
    glVertexAttribPointer(
        attribute_v_coord_postproc,  // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        0                   // offset of first element
    );
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
};
void CombinationStage::drawb()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
    glVertexAttribPointer(
        attribute_v_coord_postproc,  // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        0                   // offset of first element
    );
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
};


