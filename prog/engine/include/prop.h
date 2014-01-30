#ifndef PROP_H
#define PROP_H

#include <list>
#include <memory>
#include "object3d.h"
#include "mesh.h"

using namespace std;

/// START BY IMPLEMENTING RENDERBATCHES FOR JUST MESH

struct RenderBatch
{
    weak_ptr<Mesh> mesh_ptr;      /// non_owning, points to resource manager's (owner) unique_ptr
    // Texture* tex_ptr ///
    // BumpMap* bpm_ptr ///
    /// Add as many uniforms, and uniform samplers as needed
};

class Prop : public Object3d
{
public:
    Prop();
    virtual ~Prop();

    void attachBatch(weak_ptr<Mesh> mesh_ptr_in);

//        list<weak_ptr<Mesh>> mesh_ptrs; /// non_owning pointers, unable to delete
    list<RenderBatch> render_batches;

protected:
private:



};

#endif // PROP_H
