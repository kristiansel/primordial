#ifndef GRASSSHADER_H
#define GRASSSHADER_H

#include "shaderbase.h"
#include "geometry.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "smallvisual.h"
//#include "prop.h"

class GrassShader : public ShaderBase
{
    public:
        GrassShader();
        virtual ~GrassShader();

        void init(GLuint shadowmap_depth_texture,
                  GLuint global_uniforms_binding);

        void unload();

        void updateTransforms(const int num, glm::vec4 const * const transforms);

//        void activate(const Camera &cam_in,
//                      const glm::mat4 &light_mvp_mat);

        void draw(const Camera &cam_in,
                    const glm::mat4 &light_mvp_mat);

        void extDraw(SmallVisual &small_visual, const Camera &cam_in,
                    const glm::mat4 &light_mvp_mat);

    protected:
    private:
        struct Uniforms
        {
        // light uniforms
//        GLuint num_lights;
//        GLuint light_posns;
//        GLuint light_cols;
        //GLuint main_light_dir;
        //GLuint main_light_color;

        // material uniforms
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint shininess;
        GLuint emission;

        // "sampler" uniforms
        GLuint tex;
        GLuint shadow_depth;
        GLuint tex2;
        GLuint worldpos_tex;

        // matrices
        GLuint cam_mat;
        GLuint light_bias_mat;

        // model view matrix
        //GLuint mv_mat;

        // Object to world space matrix
        // for fog computations
        //GLuint to_world_space_mat;

        // light model view projection matrix
        //GLuint shadowmap_mvp_mat;

        GLuint globalUniformsBlockIndex;

    } uniforms;

    GLuint worldpos_tex_id;

    GLuint shadowmap_depth_texture;              // Texture

    // Actual data
    Mesh mesh;
    Texture tex;
    int num_transforms;

    bool debug;
};

#endif // GRASSSHADER_H
