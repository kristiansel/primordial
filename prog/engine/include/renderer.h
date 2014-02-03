#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include "shader.h"
#include "scene.h"

class Renderer
{
public:
    struct Perspective
    {
        Perspective();
        ~Perspective();
        explicit Perspective(float fovy, float aspect, float nearz, float farz);

        void setPerspective(float fovy_in, float aspect_in, float nearz_in, float farz_in);
        mat4 getModelViewMatrix();

        float fovy;
        float aspect;
        float nearz;
        float farz;
    };
public:
    Renderer();
    virtual ~Renderer();

    void init(unsigned int scr_width_in, unsigned int scr_height_in); // depends on an OpenGL context, therefore public
    void draw(Scene &scene);
    void resizeWindow(int w, int h, bool real = true);

protected:
private:
    Shader main_shader;

    /// Scene items, to be re-arranged later
    //Scene *scene;
    Perspective perspective;


};

#endif // RENDERER_H
