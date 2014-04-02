#include "textshader.h"

TextShader::TextShader()
{
    //ctor
}

TextShader::~TextShader()
{
    //dtor
    unload();
}

void TextShader::init()
{
    ShaderBase::load("shaders/text_vert.glsl", "shaders/text_frag.glsl");

    // set the uniform locations
    uniforms.font_tex = glGetUniformLocation(getProgramID(), "font_texture");
    glUniform1i(uniforms.font_tex, 0);            // ALWAYS CHANNEL 0

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // This shader is not the first to draw, so other enabled vertex attrib pointers should
    // be set already, thus not causing segfault


    // store internally the font image
    tex_font.fromFile("Holstein");

    // buffers
    // Initialize VBO
    glGenBuffers(1, &Text2DVertexBufferID);
    glGenBuffers(1, &Text2DUVBufferID);

}

void TextShader::printText2D(const char* text, int x, int y, int size)
{
    unsigned int length = strlen(text);

    // Fill buffers
    std::vector<glm::vec2> vertices;
    std::vector<glm::vec2> UVs;
    for ( unsigned int i=0 ; i<length ; i++ ){

        glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
        glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
        glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
        glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

        vertices.push_back(vertex_up_left   );
        vertices.push_back(vertex_down_left );
        vertices.push_back(vertex_up_right  );

        vertices.push_back(vertex_down_right);
        vertices.push_back(vertex_up_right);
        vertices.push_back(vertex_down_left);

        char character = text[i];
        float uv_x = (character%16)/16.0f;
        float uv_y = (character/16)/16.0f;

        glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
        glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
        glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
        glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
        UVs.push_back(uv_up_left   );
        UVs.push_back(uv_down_left );
        UVs.push_back(uv_up_right  );

        UVs.push_back(uv_down_right);
        UVs.push_back(uv_up_right);
        UVs.push_back(uv_down_left);
    }
    glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

    // Bind shader
    ShaderBase::switchTo();

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_font.getTBOid());
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(uniforms.font_tex, 0);

    // 1rst attribute buffer : vertices
    //glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    // 2nd attribute buffer : UVs
    //glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );


    // enables/disables
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);

    // Draw call
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

    glEnable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);

    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
}

void TextShader::unload()
{
    // deleting the shader object
    if (isLoaded())
    {
        // not sure if this is needed
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Delete buffers
        glDeleteBuffers(1, &Text2DVertexBufferID);
        glDeleteBuffers(1, &Text2DUVBufferID);
    }

    ShaderBase::unload();
}

