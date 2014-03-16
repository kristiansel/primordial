#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#include <SFML/Graphics/Image.hpp>
#include <threadingwrapper.hpp>

class Texture
{
    public:
        Texture();
        virtual ~Texture(); // Need to flesh out this

        bool fromFile(std::string filepath_in);

        GLuint getTBOid();
    protected:
    private:
        std::string      str_key;
        sf::Image   image;

        GLuint tbo_id;

        // This mutex is probably not needed...
        Mutex sharedContextLoading; // To guard against buffers being bound differently in several threads


};

#endif // TEXTURE_H
