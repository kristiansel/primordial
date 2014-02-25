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

        void activate();
        void deactivate();

        void drawActor(std::shared_ptr<Actor> actor, glm::mat4 mv);
        void drawProp(std::shared_ptr<Prop> prop, glm::mat4 mv);

//        int getWidth();
//        int getHeight();

    protected:
    private:

        //FRAMEBUFFERS
        GLuint fbo;
        GLuint tex_depth;

        //UNIFORMS
        GLuint light_mvp_mat;


        /// Helper members
        glm::mat4 depthProjectionMatrix;
        glm::mat4 depthViewMatrix;
};

#endif // SHADOWMAP_H
