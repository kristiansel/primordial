#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <glm/gtx/transform.hpp>

#include "prop.h"
#include "actor.h"
#include "shaderbase.h"

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

    void load(string vertex_shader, string fragment_shader);
    void unload();

    void activate(glm::mat4 mv, glm::mat4 light_mvp_mat, GLuint shadow_depth);

    void drawActor(shared_ptr<Actor> actor, glm::mat4 mv);
    void drawProp(shared_ptr<Prop> prop, glm::mat4 mv);
    /// void draw(Terrain &terrain, glm::mat4 mv);

    /// Should look into ridding me of this:
    GLuint getBoneMat() {return bone_mat;};

protected:
private:
    /// uniforms
    GLuint num_lights;
    GLuint light_posns;
    GLuint light_cols;

    GLuint ambient;
    GLuint diffuse;
    GLuint specular;
    GLuint shininess;
    GLuint emission;

    GLuint tex;
    GLuint shadow_depth;    /// Uniform

    GLuint bone_mat;

    GLuint mv_mat;

    GLuint shadowmap_mvp_mat;

    /// attributes
    GLuint vertex;
    GLuint normal;
    GLuint texCoord0;
    GLuint bone_index;
    GLuint bone_weight;     /// set by shader

    /// temp
    glm::mat4 light_mvp_mat;
    GLuint sdepth;          /// Texture

};

#endif // SHADER_H
