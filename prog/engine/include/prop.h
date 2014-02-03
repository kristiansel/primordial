#ifndef PROP_H
#define PROP_H

#include <list>
#include <memory>
#include "object3d.h"
#include "mesh.h"
#include "texture.h"

/// START BY IMPLEMENTING RENDERBATCHES FOR JUST MESH

struct RenderBatch
{
    std::weak_ptr<Mesh> mesh_ptr;      /// non_owning, points to resource manager's (owner) unique_ptr
    std::weak_ptr<Texture> tex_ptr; ///
    // BumpMap* bpm_ptr ///
    /// Add as many uniforms, and uniform samplers as needed
};

class Prop : public Object3d
{
public:
    Prop();
    virtual ~Prop();

    void attachBatch(std::weak_ptr<Mesh> mesh_ptr_in, std::weak_ptr<Texture> tex_ptr_in);

//        list<weak_ptr<Mesh>> mesh_ptrs; /// non_owning pointers, unable to delete
    std::list<RenderBatch> render_batches;

protected:
private:



};

#endif // PROP_H
