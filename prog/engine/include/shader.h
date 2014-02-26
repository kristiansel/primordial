#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <stdexcept>
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

//    void setLights(glm::mat4 mv); /// tobe refactored
//        void draw(Mesh &mesh, glm::mat4 mv); /// Soon to be deleted/refactored
    // void draw(Prop &prop, glm::mat4 mv);
    void drawActor(shared_ptr<Actor> actor, glm::mat4 mv);
    void drawProp(shared_ptr<Prop> prop, glm::mat4 mv);
    /// void draw(Actor &actor, glm::mat4 mv);
    /// void draw(Terrain &terrain, glm::mat4 mv);

    /// Should look into ridding me of this:
    GLuint getBoneMat() {return bone_mat;};

    /// Why are these out here in public? Should be in shader baseclass
//    static GLuint initshaders (GLenum type, const char *filename);
//    static GLuint initprogram (GLuint vertexshader, GLuint fragmentshader);
//    static string textFileRead (const char * filename);
//    static void programerrors (const GLint program);
//    static void shadererrors (const GLint shader);

protected:
private:
    /// opengl program id
    //GLuint program_id;

    /// uniforms
    GLuint num_lights;      /// set by shader.
    GLuint light_posns;     /// set by shader.
    GLuint light_cols;      /// set by shader.

    GLuint ambient;         /// set by shader
    GLuint diffuse;         /// set by shader
    GLuint specular;        /// set by shader
    GLuint shininess;       /// set by shader
    GLuint emission;        /// set by shader

//        GLuint hasTexture;      /// set by shader
    GLuint tex;             /// set by shader.
    GLuint shadow_depth;
//        GLuint tex2;            /// set by shader. NOT SET YET (SEGFAULT)   AVOIDABLE
//        GLuint time;            /// set by shader
    GLuint bone_mat;        /// set by shader.
//        GLuint nobo_mat;        /// set by shader.
    GLuint mv_mat;          /// set by shader
    //GLuint nrm_mat;         /// set by shader

    GLuint shadowmap_mvp_mat;

    /// attributes
    GLuint vertex;          /// set by shader
    GLuint normal;          /// set by shader
    GLuint texCoord0;       /// set by shader
    GLuint bone_index;      /// set by shader
    GLuint bone_weight;     /// set by shader

    /// helper functions:

    //void unload();

    /// helper variables
    //bool loaded;

    /// temp
    glm::mat4 light_mvp_mat;
    GLuint sdepth;

};

#endif // SHADER_H
