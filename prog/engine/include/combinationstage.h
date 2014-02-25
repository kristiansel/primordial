#ifndef COMBINATIONSTAGE_H
#define COMBINATIONSTAGE_H

//#define GLEW_STATIC
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <GL/glew.h>
//#include <GL/glut.h>
#include <vector>
#include "shader.h"
#include "shaderbase.h"

class CombinationStage : public ShaderBase
{
public:
    CombinationStage();
    virtual ~CombinationStage();
//    void init(int w, int h, char* shader_path);
    void init(int w, int h);
    void draw();
    void drawb();

    void activate(float w0, float w1);
    void activateTextures(GLuint color0, GLuint color1);

    GLuint attribute_v_coord_postproc;

    GLuint fbo;
    GLuint fbo_texture;
    GLuint rbo_depth;

    GLuint vbo_fbo_vertices;
//    GLuint program_postproc;


    GLuint uniform_w;
    GLuint uniform_textures[2];
    GLuint textures[2];

//    bool loaded;
};

#endif // COMBINATIONSTAGE_H
