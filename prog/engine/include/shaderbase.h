#ifndef SHADERBASE_H
#define SHADERBASE_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <stdexcept>

class ShaderBase
{
    public:
        ShaderBase();
        virtual ~ShaderBase(); // confirm that this is called by derived classes

        void switchTo();

    protected:
        // Specialized derived shaders with fixed programs
        // call this
        void load(std::string vertex_shader, std::string fragment_shader);
        void unload();
        void validate();
    public:
        static GLuint initshaders (GLenum type, const char *filename);
        static GLuint initprogram (GLuint vertexshader, GLuint fragmentshader);
        static std::string textFileRead (const char * filename);
        static void programerrors (const GLint program);
        static void shadererrors (const GLint shader);

    protected:
        GLuint getProgramID();
        bool isLoaded();

    private:
    //protected:
        GLuint program_id;
        bool loaded;
};

#endif // SHADERBASE_H
