#ifndef POSTPROCSTAGE_H
#define POSTPROCSTAGE_H

//#define GLEW_STATIC
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <GL/glew.h>
#include <vector>
#include "shader.h"
#include "shaderbase.h"

class PostProcStage : public ShaderBase
{
public:
    PostProcStage();
    virtual ~PostProcStage();

    void init(int w, int h, char const *shader_path);
    void activate(int size, float* kern_x, float* kern_y);
    void resize(int w, int h);
    void drawb();
    void draw();

    //void bindResultingTexture(int channel);
    void activateTextures(GLuint color, GLuint depth);

    GLuint fbo;
    GLuint fbo_texture;
    GLuint rbo_depth;

    GLuint vbo_fbo_vertices;

    //GLuint program_postproc;
    GLint attribute_v_coord_postproc;

    GLint uniform_fbo_depth;
    GLint uniform_fbo_texture;
    GLint uniform_kern_x;
    GLint uniform_kern_y;

//        std::vector<GLuint> uniforms;
//        std::vector<GLuint> attributes;

    //bool loaded;

protected:
private:
};



#endif // POSTPROCSTAGE_H
