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

        void init(GLuint global_uniforms_binding);

        void draw(const SmallVisual &small_visual, glm::mat4 &light_vp);


    protected:
    private:
        struct Uniforms
        {
            GLuint worldpos_tex;
            GLuint light_mvp;
            GLuint tex;

            GLuint wind_params;

            GLuint globalUniformsBlockIndex;
        } uniforms;
};

#endif // SHADOWINSTSHADER_H
