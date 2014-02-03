#include "texture.h"

Texture::Texture()
{
    //ctor
}

Texture::~Texture()
{
    /// release RAM pointers
    /// Assume sf::Image releases itself

    /// release video RAM buffers
    glBindTexture(GL_TEXTURE_2D, 0); /// Really this should not be necessary

//    cout << "Deleting Buffers: " << vbo_id << " & " << ibo_id << "\n";
    glDeleteTextures(1, &tbo_id);
}

bool Texture::fromFile(string filepath_in)
{
    filepath_in = "assets_raw/textures/"+filepath_in+".png";

    if (!(image.loadFromFile(filepath_in)))
    {
        cout << "unable to load texture: " << filepath_in << "\n";
        return false;
    }
    else
    {
        //            std::cout << "loaded!\n";

        glGenTextures(1, &tbo_id);
        //        cout<<"texture id: "<<tbo_id<<"\n";

        //glActiveTexture( GL_TEXTURE0 );
        glBindTexture(GL_TEXTURE_2D, tbo_id);
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        sf::Vector2u tsize = image.getSize();
        //        std::cout<<"texture size = "<<size.x<<", "<<size.y<<"\n";
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tsize.x, tsize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr() );
    //    glGenerateMipmap(GL_TEXTURE_2D)

    //    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, size.x, size.y,
    //                      GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());

        glBindTexture(GL_TEXTURE_2D, 0);

//        GLenum errCode;
//        const GLubyte *errString;

//        if ((errCode = glGetError()) != GL_NO_ERROR) {
//        errString = gluErrorString(errCode);
//        fprintf (stderr, "OpenGL Error: %s\n", errString);

        //assuming loading went well if we get here
        return true;
    }

}

GLuint Texture::getTBOid()
{
    return tbo_id;
}
