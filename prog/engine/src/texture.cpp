#include "texture.h"

Texture::Texture() : load_stage(NotLoaded), tbo_id(99999)
{
    //ctor
}

Texture::~Texture()
{
    deleteGL();
}

bool Texture::fromFile(std::string filepath_in)
{
    filepath_in = "assets/textures/"+filepath_in+".png";

    //std::cout << "loading texture " << filepath_in << "\n";

    if (!(image.loadFromFile(filepath_in)))
    {
        // std::cerr << "unable to load texture: " << filepath_in << "\n";
        return false;
    }
    else
    {
        createGL();

        // assuming loading went well if we get here
        return true;
    }

}

GLuint Texture::getTBOid()
{
    return tbo_id;
}

void Texture::deleteGL()
{
    glBindTexture(GL_TEXTURE_2D, 0); // Really this should not be necessary

    //std::cout << "deleting texture, tbo_id: " << tbo_id << "\n";

    glDeleteTextures(1, &tbo_id);

    load_stage = NotLoaded;
}

void Texture::createGL()
{
    glGenTextures(1, &tbo_id);
    //        cout<<"texture id: "<<tbo_id<<"\n";

    //glActiveTexture( GL_TEXTURE0 );
    glBindTexture(GL_TEXTURE_2D, tbo_id);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR); // TRILINEAR FILTERING
//        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_NEAREST); // BILINEAR FILTERING
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST); // NEAREST NEIGHBOR FILTERING


    sf::Vector2u tsize = image.getSize();
    //        std::cout<<"texture size = "<<size.x<<", "<<size.y<<"\n";
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tsize.x, tsize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr() );
    glGenerateMipmap(GL_TEXTURE_2D);    // Needed for texture filtering

//    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, size.x, size.y,
//                      GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());

    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum errCode;
    errCode = glGetError();
    if (errCode != GL_NO_ERROR) {
        std::cerr << "glGetError()) != GL_NO_ERROR-----TEXTURE------\n";
        std::cerr << "errCode: " << errCode << "\n";

        if (errCode == GL_INVALID_VALUE)
        {
            std::cerr << "GL_INVALID_VALUE\n";
        }
    }

    std::cout << "creating texture buffer " << tbo_id << "\n";

    load_stage = Loaded;
}
