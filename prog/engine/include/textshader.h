#ifndef TEXTSHADER_H
#define TEXTSHADER_H

#include "shaderbase.h"
#include "texture.h"
#include <vector>
#include <glm/glm.hpp>

class TextShader : public ShaderBase
{
    public:
        TextShader();
        virtual ~TextShader();

        void init();
        void unload();

        void printText2D(const char * text, int x, int y, int size);

    protected:
    private:
        struct Uniforms
        {
            GLuint font_tex;
        } uniforms;

        // buffers
        GLuint Text2DVertexBufferID;
        GLuint Text2DUVBufferID;

        Texture tex_font;
};

#endif // TEXTSHADER_H
