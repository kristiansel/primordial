#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include "shader.h"
#include "scene.h"

///// to be re-evaluated
////#include "mesh.h"
////#include "camera.h"

using namespace std;

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    void init(Scene &scene_in, unsigned int scr_width_in, unsigned int scr_height_in); // depends on an OpenGL context, therefore public
    void draw();
    void resizeWindow(int w, int h, bool real = true);

protected:
private:
    Shader main_shader;

    /// Scene items, to be re-arranged later
    Scene *scene;


};

#endif // RENDERER_H
