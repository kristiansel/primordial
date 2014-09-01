#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "prop.h"
#include "actor.h"
#include "shaderbase.h"
#include "light.h"
#include "camera.h"

using std::shared_ptr;
using std::string;
using std::cerr;

class Shader : public ShaderBase
//class Shader
{
public:
    Shader();
    explicit Shader(string vertex_shader, string fragment_shader);
    virtual ~Shader();

    void init(GLuint shadowmap_depth_texture,
              GLuint global_uniforms_binding);

    void unload();

    void activate(const Camera &cam_in,
                  const glm::vec4 fog_color,
                  const glm::vec4 sky_color,
                  const glm::mat4 &light_mvp_mat,
                  const DirLight &main_light);

    void clearBoneMatrices();

    void drawActor(shared_ptr<Actor> actor);
    void drawProp(shared_ptr<Prop> prop, bool debug = false);

    // void draw(Terrain &terrain, glm::mat4 mv);

    // Should look into ridding me of this:
    GLuint getBoneMat() {return uniforms.bone_mat;};

protected:
private:

    // uniforms
    struct Uniforms
    {
        // light uniforms
//        GLuint num_lights;
//        GLuint light_posns;
//        GLuint light_cols;
        GLuint main_light_dir;
        GLuint main_light_color;

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

        // bone matrices
        GLuint bone_mat;

        // model view matrix
        GLuint mv_mat;

        // projection matrix
        GLuint proj_mat;

        // Object to world space matrix
        // for fog computations
        GLuint to_world_space_mat;

        // Other fog uniforms
        GLuint fog_color;
        GLuint sky_color;
        GLuint zfar;

        // light model view projection matrix
        GLuint shadowmap_mvp_mat;

//        struct BufferObjectData
//        {
//            glm::mat4 proj_matUni;
//            glm::vec4 fog_colorUni;
//            glm::vec4 sky_colorUni;
//            glm::vec4 main_light_colorUni;
//            glm::vec3 main_light_dirUni;         // does this align?
//            float zfarUni;
//        } bufferObjectData;
//
//        struct BufferObjects
//        {
//            struct BufferObjectInstance
//            {
//                GLuint ubo_id;
//                size_t ubo_size;
//                GLuint binding_index;
//                GLuint block_index;
//            } globalUniforms;
//            // BufferObjectInstance globalUniforms2;
//            // BufferObjectInstance globalUniforms3;
//            // BufferObjectInstance globalUniforms4;
//        } bufferObjects;
        GLuint globalUniformsBlockIndex;

    } uniforms;


    // attributes
    struct Attributes
    {
        GLuint vertex;
        GLuint normal;
        GLuint texCoord0;
        GLuint bone_index;
        GLuint bone_weight;
    } attributes;


    // Internally stored, so they do not have to be
    // passed as arguments every single draw call
    glm::vec4 main_light_dir;
    glm::vec4 main_light_color;
    glm::mat4 main_light_mvp_mat;
    glm::mat4 view_mat;

    // Updated on initialization
    GLuint shadowmap_depth_texture;              // Texture
    glm::mat4 clear_matrices[MAX_BONE_NUM];
};

#endif // SHADER_H
