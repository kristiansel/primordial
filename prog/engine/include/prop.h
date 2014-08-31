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
                glm::mat4 transf_mat_in,
                Object3d &obj3dIn,
                bool instanced);

    // Change to shared_ptr one day
    std::weak_ptr<Mesh> mesh_ptr;      // non_owning, points to resource manager's (owner) unique_ptr
    std::weak_ptr<Texture> tex_ptr; //
    glm::mat4 transf_mat;
    int parent_bone;
    // BumpMap* bpm_ptr //
    // Add as many uniforms, and uniform samplers as needed

    // instancing
    Object3d* addInstance();
    int getNumInstances();
    bool m_instanced;

    // individual transforms
    std::vector <Object3d> transforms;

    // working memory for shader
    std::vector<glm::mat4> mvp_mats;
    std::vector<glm::mat4> toWS_mats;
    // std::vector<glm::mat4> sm_mats;
    // std::vector<MatCol> mtl_cols;
    // std::vector<float> mtl_shiny;
};

class Prop : virtual public Object3d
{
public:
    Prop();
    virtual ~Prop();

    RenderBatch* attachBatch(std::weak_ptr<Mesh> mesh_ptr_in,
                     std::weak_ptr<Texture> tex_ptr_in, bool instanced = false/*,
                     glm::mat4 transf_mat_in = glm::mat4(1.0)*/);


    static const int render_batches_capacity = 30;

    std::list<RenderBatch> render_batches;

protected:
private:



};

#endif // PROP_H
