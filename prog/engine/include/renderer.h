#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>

#include "shader.h"
#include "grassshader.h"
#include "skyshader.h"
#include "world.h"
#include "shadowmap.h"
#include "shadowinstshader.h"
#include "renderstage.h"
#include "postprocstage.h"
#include "combinationstage.h"
#include "textshader.h"
#include "uniformbufferobject.h"
#include "foliage.h" // this should not be here


class Renderer
{
public:
    struct Settings // is this used?
    {
        int width;
        int height;
    };

    struct InterfaceInfo
    {
        float health;
        float frames_per_second;
    };
public:
    Renderer();
    virtual ~Renderer();

    void init(unsigned int scr_width_in, unsigned int scr_height_in); // depends on an OpenGL context, therefore public
    void draw(World &world, float dt);
    void drawOverlay(InterfaceInfo interfaceInfo);
    void resizeWindow(int w, int h, bool real = true);
    void setPerspective(int w, int h);

    void updateTime(float dt);

protected:
    // Helper functions
    static glm::mat4 getModelViewMatrix(Object3d o3d_in);

private:
    Shader main_shader;
    GrassShader grass_shader;
    SkyShader sky_shader;

    // Have this not change with resize
//    Perspective perspective;

    // For post processing
    Settings settings;

    static const unsigned short KERNEL_SIZE = 5;
    static const int ratio = 4; // downscaling ratio used for pp

    float pix_tex_coord_offset[2];
    float kernelOffsetx[KERNEL_SIZE*KERNEL_SIZE];
    float kernelOffsety[KERNEL_SIZE*KERNEL_SIZE];

    UniformBufferObject ubos;

    ShadowMap shadow_map;
    ShadowInstShader shadow_inst;
    RenderStage render_stage;
    PostProcStage blur1;
    PostProcStage blur2; // UNCOMMENTING THIS MAKES ONE OF THE HEAD VERTICES WEIRD...
    CombinationStage comb1;

    TextShader text_shader;
    void labelNumberPair(const char * label, float number, char * out);

    void updateKernel();

    float time;

};

#endif // RENDERER_H
