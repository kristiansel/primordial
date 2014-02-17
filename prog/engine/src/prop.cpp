#include "prop.h"

Prop::Prop()
{
    //ctor
}

Prop::~Prop()
{
    //dtor
}

void Prop::attachBatch(std::weak_ptr<Mesh> mesh_ptr_in,
                       std::weak_ptr<Texture> tex_ptr_in,
                       glm::mat4 transf_mat_in)
{
//    mesh_ptrs.push_back(mesh_ptr_in);
    render_batches.push_back( RenderBatch(mesh_ptr_in,
                                          tex_ptr_in,
                                          transf_mat_in) );
}
