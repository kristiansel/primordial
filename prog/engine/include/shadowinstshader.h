#ifndef SHADOWINSTSHADER_H
#define SHADOWINSTSHADER_H

#include "shaderbase.h"
#include "geometry.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "smallvisual.h"

class ShadowInstShader : public ShaderBase
{
    public:
        ShadowInstShader();
        virtual ~ShadowInstShader();

        void init();

        void draw(SmallVisual &small_visual, glm::mat4 &light_vp);


    protected:
    private:
        struct Uniforms
        {
            GLuint worldpos_tex;
            GLuint light_mvp;
            GLuint tex;
        } uniforms;
};

#endif // SHADOWINSTSHADER_H
