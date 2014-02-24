#ifndef RENDERSTAGE_H
#define RENDERSTAGE_H



//#define GLEW_STATIC
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <GL/glew.h>
//#include <GL/glut.h>
#include <vector>
#include "shader.h"
//#include "Transform.h"

class RenderStage
{
public:
    RenderStage();
    virtual ~RenderStage();

    void init(int w, int h);

    void resize(int w, int h);

    void activate();
    void deactivate();

    //FRAMEBUFFERS
    GLuint fbo, fbo_depth, fbo_texture;

    bool loaded;

protected:
private:
};

#endif // RENDERSTAGE_H
