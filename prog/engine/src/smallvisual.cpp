#include "smallvisual.h"
SmallVisual::SmallVisual() : updated(false)
{

}

SmallVisual::~SmallVisual()
{
//    glBindTexture(GL_TEXTURE_1D, 0); // Really this should not be necessary
//
//    glDeleteTextures(1, &worldpos_tex_id);
//
//    std::cout << "deleting smallvisual\n";
}

void SmallVisual::init()
{
    glGenTextures(1, &worldpos_tex_id);

    glBindTexture(GL_TEXTURE_1D, worldpos_tex_id);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST);
}


void SmallVisual::cleanUp()
{
    glBindTexture(GL_TEXTURE_1D, 0); // Really this should not be necessary

    glDeleteTextures(1, &worldpos_tex_id);

    std::cout << "deleting smallvisual\n";
}

void SmallVisual::updatePositionsTex() const
{
    updated = true;
    glBindTexture(GL_TEXTURE_1D, worldpos_tex_id);
    //glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, positions.size(), 0, GL_RGBA, GL_FLOAT, &(positions[0]));
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, num_smvis, 0, GL_RGBA, GL_FLOAT, &(sm_buffer[0]));
}
