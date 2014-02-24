#include "postprocstage.h"

PostProcStage::PostProcStage()
{
    loaded = false;
};

void PostProcStage::init(int w, int h, char* shader_path)
{
    //TEXTURE
    //glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &fbo_texture);
//    cout<<"pp_fbo_texture id: "<<fbo_texture<<"\n";
    glBindTexture(GL_TEXTURE_2D, fbo_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    //DEPTH BUFFER (RENDER)
    glGenRenderbuffers(1, &rbo_depth);
//    cout<<"pp_rbo_depth id: "<<rbo_depth<<"\n";
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

    // Post-processing
    GLuint vs, fs;
    GLint link_ok;
    GLint validate_ok;

    vs = Shader::initshaders(GL_VERTEX_SHADER, "shaders/pp_wave.vert.glsl");
    fs = Shader::initshaders(GL_FRAGMENT_SHADER, shader_path);

    program_postproc = glCreateProgram();
    glAttachShader(program_postproc, vs);
    glAttachShader(program_postproc, fs);
    glLinkProgram(program_postproc);
    glGetProgramiv(program_postproc, GL_LINK_STATUS, &link_ok);
    if (!link_ok)
    {
        fprintf(stderr, "glLinkProgram:");
        Shader::shadererrors(program_postproc);
        //return 0;
    }
    glValidateProgram(program_postproc);
    glGetProgramiv(program_postproc, GL_VALIDATE_STATUS, &validate_ok);
    if (!validate_ok)
    {
        fprintf(stderr, "glValidateProgram:");
        //print_log(program_postproc);
    }

    // get/enable attribs/uniforms and everything
    char* attribute_name = "v_coord";
    attribute_v_coord_postproc = glGetAttribLocation(program_postproc, attribute_name);
    if (attribute_v_coord_postproc == -1)
    {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
//        return 0;
    }

    char* uniform_name = "fbo_texture";
    uniform_fbo_texture = glGetUniformLocation(program_postproc, uniform_name);
    uniform_fbo_depth = glGetUniformLocation(program_postproc, "rbo_DepthTexture");
    uniform_kern_x = glGetUniformLocation(program_postproc, "x_offsets");
    uniform_kern_y = glGetUniformLocation(program_postproc, "y_offsets");

    int isValid;
    glValidateProgram(program_postproc);
    glGetProgramiv(program_postproc, GL_VALIDATE_STATUS, &isValid);

    if (uniform_fbo_texture == -1)
    {
        fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
        // return 0;
    }

    glEnableVertexAttribArray(attribute_v_coord_postproc);
    loaded = true;
}

void PostProcStage::resize(int w, int h)
{
    glBindTexture(GL_TEXTURE_2D, fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void PostProcStage::activate(int size, float* kern_x, float* kern_y)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glUseProgram(program_postproc);

    glUniform1fv(uniform_kern_x, size*size, kern_x);
    glUniform1fv(uniform_kern_y, size*size, kern_y);
}

void PostProcStage::activateTextures(GLuint color, GLuint depth)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depth);

    glUniform1i(uniform_fbo_texture, 0);
    glUniform1i(uniform_fbo_depth, 1);

}

void PostProcStage::drawb()
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
    //      glDisableVertexAttribArray(attribute_v_coord_postproc);

    //glUseProgram(shaderprogram);

    //glutSwapBuffers(); // this should only be called in the last pass;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void PostProcStage::draw()
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
    //      glDisableVertexAttribArray(attribute_v_coord_postproc);

    //glUseProgram(shaderprogram);

//(); // this should only be called in the last pass;
};

PostProcStage::~PostProcStage()
{
    if (loaded)
    {
        cout<<"deleting post-processing stage buffers\n";
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteTextures(1, &fbo_texture);
        glDeleteFramebuffers(1, &fbo);
        glDeleteRenderbuffers(1, &rbo_depth);
        glDisableVertexAttribArray(attribute_v_coord_postproc);
    }
}
