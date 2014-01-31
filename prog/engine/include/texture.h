#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include <string>
#include <iostream>
#include <SFML/Graphics/Image.hpp>

using namespace std;

class Texture
{
    public:
        Texture();
        virtual ~Texture();

        bool fromFile(string filepath_in);

        GLuint getTBOid();
    protected:
    private:
        string      str_key;
        sf::Image   image;

        GLuint tbo_id;
};

#endif // TEXTURE_H
