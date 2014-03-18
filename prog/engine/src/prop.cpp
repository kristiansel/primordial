#include "prop.h"

Prop::Prop()
{
    // Reserve space for the render batches
//    render_batches.reserve(render_batches_capacity);
}

Prop::~Prop()
{
    //dtor
}

RenderBatch* Prop::attachBatch(std::weak_ptr<Mesh> mesh_ptr_in,
                       std::weak_ptr<Texture> tex_ptr_in,
                       glm::mat4 transf_mat_in)
{
    if (!(render_batches.size()<render_batches_capacity))
    {
        std::cout << "warning: exceeding capacity of renderbatches. \nReallocation may occur\n";
    }

    render_batches.push_back( RenderBatch(mesh_ptr_in,
                                          tex_ptr_in,
                                          transf_mat_in) );
    return &render_batches.back();
}
