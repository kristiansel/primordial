#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <stdexcept>

#include "prop.h"

using std::shared_ptr;
using std::string;
using std::cerr;

class Shader
{
public:
    Shader();
    explicit Shader(string vertex_shader, string fragment_shader);
    virtual ~Shader();

    void load(string vertex_shader, string fragment_shader);
    void switchTo();
    void setLights(mat4 mv); /// tobe refactored
//        void draw(Mesh &mesh, mat4 mv); /// Soon to be deleted/refactored
    void draw(Prop &prop, mat4 mv);
    void draw(shared_ptr<Prop> prop, mat4 mv);
    /// void draw(Actor &actor, mat4 mv);
    /// void draw(Terrain &terrain, mat4 mv);

protected:
private:
    /// opengl program id
    GLuint program_id;

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
        GLuint tex;             /// set by shader. NOT SET YET (SEGFAULT)   AVOIDABLE
//        GLuint tex2;            /// set by shader. NOT SET YET (SEGFAULT)   AVOIDABLE
//        GLuint time;            /// set by shader
//        GLuint bone_mat;        /// set by shader.
//        GLuint nobo_mat;        /// set by shader. NOT SET YET (SEGFAULT)   AVOIDABLE
    GLuint mv_mat;          /// set by shader
    GLuint nrm_mat;         /// set by shader

    /// attributes
    GLuint vertex;          /// set by shader
    GLuint normal;          /// set by shader
    GLuint texCoord0;       /// set by shader
//        GLuint bone_index;      /// set by shader
//        GLuint bone_weight;     /// set by shader

    /// helper functions:
    static string textFileRead (const char * filename);
    static void programerrors (const GLint program);
    static void shadererrors (const GLint shader);
    static GLuint initshaders (GLenum type, const char *filename);
    static GLuint initprogram (GLuint vertexshader, GLuint fragmentshader);
    void unload();

    /// helper variables
    bool loaded;

};

#endif // SHADER_H
