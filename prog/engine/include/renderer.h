#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shader.h"
#include "skyshader.h"
#include "scene.h"
#include "shadowmap.h"
#include "renderstage.h"
#include "postprocstage.h"
#include "combinationstage.h"
#include "textshader.h"


class Renderer
{
public:
    struct Settings
    {
        int width;
        int height;
    };
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
    void draw(Scene &scene, float dt);
    void resizeWindow(int w, int h, bool real = true);
    void setPerspective(int w, int h);

protected:
    // Helper functions
    static glm::mat4 getModelViewMatrix(Object3d o3d_in);

private:
    Shader main_shader;
    SkyShader sky_shader;

    // Have this not change with resize
    Perspective perspective;

    // For post processing
    Settings settings;

    static const unsigned short KERNEL_SIZE = 5;
    static const int ratio = 4; // downscaling ratio used for pp

    float pix_tex_coord_offset[2];
    float kernelOffsetx[KERNEL_SIZE*KERNEL_SIZE];
    float kernelOffsety[KERNEL_SIZE*KERNEL_SIZE];

    ShadowMap shadow_map;
    RenderStage render_stage;
    PostProcStage blur1;
    PostProcStage blur2; // UNCOMMENTING THIS MAKES ONE OF THE HEAD VERTICES WEIRD...
    CombinationStage comb1;

    TextShader text_shader;

    void updateKernel();

};

#endif // RENDERER_H
