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
                  const glm::mat4 &light_mvp_mat);

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

        // Object to world space matrix
        // for fog computations
        GLuint to_world_space_mat;

        // light model view projection matrix
        GLuint shadowmap_mvp_mat;

        // Block "pointer" index to global uniforms
        GLuint globalUniformsBlockIndex;

    } uniforms;

    // Internally stored, so they do not have to be
    // passed as arguments every single draw call
//    glm::vec4 main_light_dir;
//    glm::vec4 main_light_color;
    glm::mat4 main_light_mvp_mat;
    glm::mat4 view_mat;

    // Updated on initialization
    GLuint shadowmap_depth_texture;              // Texture
    glm::mat4 clear_matrices[MAX_BONE_NUM];
};

#endif // SHADER_H
