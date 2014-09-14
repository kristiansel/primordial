#ifndef SMALLVISUAL_H
#define SMALLVISUAL_H

#include <memory>
#include <vector>
#include <string>
#include "mesh.h"
#include "texture.h"
//#include "threadingwrapper.h"
#include "quadtree.hpp"
#include "boost/thread.hpp"
#include "global.h"

//#include <thread>
//#include <mutex>


using Mutex = boost::mutex;
using LockGuard = boost::lock_guard<boost::mutex>;

//using Mutex = std::mutex;
//using LockGuard = std::lock_guard<std::mutex>;

struct SmallVisual
{
    SmallVisual();
    SmallVisual(const SmallVisual&);
    ~SmallVisual();

    void init();
    void cleanUp();

    void init(std::string mesh_key, std::string tex_key, glm::vec4 wind_params_in);
    //void splitRange(std::string mesh_key, std::string tex_key, glm::vec4 wind_params, float draw_range_in);

    void updatePositionsTex() const;

    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> tex;
    //std::vector<glm::vec4> positions;

    static const unsigned int MAX_NUM_SMVIS = 5000;
    unsigned int num_smvis;
    glm::vec4 sm_buffer[MAX_NUM_SMVIS];

//    // for split range
//    bool split_range;
//    unsigned int num_smvis_far;
//    glm::vec4 sm_buffer_far[MAX_NUM_SMVIS];
//    float draw_range_far;

    mutable bool updated;

    glm::vec4 wind_params;

    GLuint worldpos_tex_id;

    // spatial indexing and concurrency
//    Mutex sv_mutex;
//    QuadTree<glm::vec4, 5> sv_qtree;
};


#endif // SMALLVISUAL_H
