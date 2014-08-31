#include "prop.h"

RenderBatch::RenderBatch(std::weak_ptr<Mesh> mesh_ptr_in,
                         std::weak_ptr<Texture> tex_ptr_in,
                         glm::mat4 transf_mat_in,
                         Object3d &obj3dIn, bool instanced) :

    mesh_ptr(mesh_ptr_in),
    tex_ptr(tex_ptr_in),
    transf_mat(transf_mat_in),
    parent_bone(-1)
{
    m_instanced = instanced;

    transforms.push_back(obj3dIn);

    mvp_mats.push_back(glm::mat4(1.0));
    toWS_mats.push_back(glm::mat4(1.0));
}

Object3d* RenderBatch::addInstance()
{
    transforms.push_back(Object3d());

    mvp_mats.push_back(glm::mat4(1.0));
    toWS_mats.push_back(glm::mat4(1.0));

    return &(transforms.back());
}

int RenderBatch::getNumInstances()
{
    return transforms.size();
}

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
                       std::weak_ptr<Texture> tex_ptr_in, bool instanced/*,
                       glm::mat4 transf_mat_in*/)
{
    if (!(render_batches.size()<render_batches_capacity))
    {
        std::cout << "warning: exceeding capacity of renderbatches. \nReallocation may occur\n";
    }

    render_batches.push_back( RenderBatch(mesh_ptr_in,
                                          tex_ptr_in,
                                          glm::mat4(1.0),
                                          (*this), instanced));
    return &render_batches.back();
}
