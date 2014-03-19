#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include "shaderbase.h"
#include "actor.h"
#include "prop.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class ShadowMap : public ShaderBase
{
    public:
        ShadowMap();
        virtual ~ShadowMap();

        void init();

        void activate(const glm::mat4 &light_vp);                // Draws to frame buffer object
        void activateDrawContent(const glm::mat4 &light_vp);     // Draws directly to screen buffer, needs update in case of debug

        void clearBoneMatrices();

        void drawProp(std::shared_ptr<Prop> prop);
        void drawActor(std::shared_ptr<Actor> actor);


        void deactivate();



//        int getWidth();
//        int getHeight();

        GLuint getDepthTex() {return tex_depth;};
        glm::mat4 getLightMVPmat() {return light_vp_value;};

    protected:
    private:

        // Uniforms
        struct Uniforms
        {
            GLuint light_mvp_mat;
            GLuint bone_mat;
        } uniforms;

        // Attributes
        struct Attributes
        {
            GLuint vertex;
            GLuint bone_index;
            GLuint bone_weight;
        } attributes;

        // Frame buffer info
        GLuint fbo;
        GLuint tex_depth;

        // test vertices
        GLuint vbo_fbo_vertices;

        // Internally stored, so they do not have to be
        // passed as arguments every single draw call
        glm::mat4 light_vp_value;
        glm::mat4 clear_matrices[MAX_BONE_NUM];


        int triggered;
};

#endif // SHADOWMAP_H
