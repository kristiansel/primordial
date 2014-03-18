#ifndef PROP_H
#define PROP_H

#include <list>
#include <memory>
#include "object3d.h"
#include "mesh.h"
#include "texture.h"

// START BY IMPLEMENTING RENDERBATCHES FOR JUST MESH

struct RenderBatch
{
    RenderBatch(std::weak_ptr<Mesh> mesh_ptr_in,
                std::weak_ptr<Texture> tex_ptr_in,
                glm::mat4 transf_mat_in) :
                    mesh_ptr(mesh_ptr_in),
                    tex_ptr(tex_ptr_in),
                    transf_mat(transf_mat_in),
                    parent_bone(-1) {};

    // Change to shared_ptr one day
    std::weak_ptr<Mesh> mesh_ptr;      // non_owning, points to resource manager's (owner) unique_ptr
    std::weak_ptr<Texture> tex_ptr; //
    glm::mat4 transf_mat;
    int parent_bone;
    // BumpMap* bpm_ptr //
    // Add as many uniforms, and uniform samplers as needed
};

class Prop : virtual public Object3d
{
public:
    Prop();
    virtual ~Prop();

    RenderBatch* attachBatch(std::weak_ptr<Mesh> mesh_ptr_in,
                     std::weak_ptr<Texture> tex_ptr_in,
                     glm::mat4 transf_mat_in = glm::mat4(1.0));


    static const int render_batches_capacity = 30;

    std::list<RenderBatch> render_batches;

protected:
private:



};

#endif // PROP_H
