#include "smallvisual.h"
SmallVisual::SmallVisual() :
    //sv_qtree(QuadAABB({-500.0, 500.0, -500.0, 500.0})),
    num_smvis(0)//,
    //num_smvis_far(0),
    //splitrange(false)
{

}
//
//SmallVisual::SmallVisual(const SmallVisual& in) :
//    //sv_mutex(), // non-copyable
//    //sv_qtree(in.sv_qtree),
//    num_smvis(in.num_smvis)//,
//    //num_smvis_far(in.num_smvis_far),
//    //splitrange(in.splitrange)
//{
//
//}

SmallVisual::~SmallVisual()
{
    // deleted because it was in a vector
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

void SmallVisual::init(std::string mesh_key, std::string tex_key, glm::vec4 wind_params_in)
{
    glGenTextures(1, &worldpos_tex_id);

    glBindTexture(GL_TEXTURE_1D, worldpos_tex_id);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST);

    mesh = std::shared_ptr<Mesh>(global::mesh_manager.getResptrFromKey(mesh_key));
    tex = std::shared_ptr<Texture>(global::tex_manager.getResptrFromKey(tex_key));
    wind_params = wind_params_in;
}

//void SmallVisual::splitRange(std::string mesh_key, std::string tex_key, glm::vec4 wind_params, float draw_range)
//{
//
//}
