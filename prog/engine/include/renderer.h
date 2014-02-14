#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include "shader.h"
#include "scene.h"
#include <glm/gtc/matrix_transform.hpp>

class Renderer
{
public:
    struct Perspective
    {
        Perspective();
        ~Perspective();
        explicit Perspective(float fovy, float aspect, float nearz, float farz);

        void setPerspective(float fovy_in, float aspect_in, float nearz_in, float farz_in);

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
    /// Helper functions
    static glm::mat4 getModelViewMatrix(Object3d o3d_in);

private:
    Shader main_shader;

    /// Scene items, to be re-arranged later
    //Scene *scene;
    Perspective perspective;


};

#endif // RENDERER_H
