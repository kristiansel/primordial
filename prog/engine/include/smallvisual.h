#ifndef SMALLVISUAL_H
#define SMALLVISUAL_H

#include <memory>
#include <vector>
#include "mesh.h"
#include "texture.h"
//#include "threadingwrapper.h"
#include "quadtree.hpp"
#include "boost/thread.hpp"

using Mutex = boost::mutex;
using LockGuard = boost::lock_guard<boost::mutex>;

struct SmallVisual
{
    SmallVisual() : sv_qtree(QuadAABB({-500.0, 500.0, -500.0, 500.0})) {}
    SmallVisual(const SmallVisual&) : sv_mutex(), sv_qtree(QuadAABB({-500, 500, -500, 500})) {}
    ~SmallVisual();

    void init();
    void cleanUp();

    void init(std::string mesh_key, std::string tex_key, glm::vec4 wind_params, float draw_range);
    void splitRange(std::string mesh_key, std::string tex_key, glm::vec4 wind_params, float draw_range);

    void updatePositionsTex() const;

    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> tex;
    //std::vector<glm::vec4> positions;

    static const unsigned int MAX_NUM_SMVIS = 5000;
    unsigned int num_smvis;
    glm::vec4 sm_buffer[MAX_NUM_SMVIS];

    mutable bool updated;

    glm::vec4 wind_params;

    GLuint worldpos_tex_id;

    // spatial indexing and concurrency
    Mutex sv_mutex;
    QuadTree<glm::vec4, 5> sv_qtree;
//
//private:
//    SmallVisual(const SmallVisual&);
};


#endif // SMALLVISUAL_H
