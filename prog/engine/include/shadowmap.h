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

        void activate();                /// Draws to frame buffer object
        void activateDrawContent();     /// Draws directly to screen buffer

        void deactivate();

        void drawActor(std::shared_ptr<Actor> actor);
        void drawProp(std::shared_ptr<Prop> prop);

//        int getWidth();
//        int getHeight();

        GLuint getDepthTex() {return tex_depth;};
        glm::mat4 getLightMVPmat() {return light_mvp_mat_value;};

    protected:
    private:

        ///FRAMEBUFFERS
        GLuint fbo;
        GLuint tex_depth;

        ///UNIFORMS
        GLuint light_mvp_mat;

        ///Attributes
        GLuint vertex;


        /// Helper members
        glm::mat4 light_mvp_mat_value;
};

#endif // SHADOWMAP_H
