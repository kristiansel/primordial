#include "backgroundmaster.h"

BackGroundMaster::BackGroundMaster(World *world_in) :
    world(world_in),
    test_var(4.0)
{
    //ctor
    for (int i = 0; i<D_PATCH_NUM; i++)
    {
        float x = (rand()%1000*D_PATCH_SCALE)/1000.f-D_PATCH_SCALE/2.f;
        float z = (rand()%1000*D_PATCH_SCALE)/1000.f-D_PATCH_SCALE/2.f;
        discrete_patch[i] = glm::vec4(x, 0.0, z, 1.0);
    }
}

BackGroundMaster::~BackGroundMaster()
{
    //dtor
}

void BackGroundMaster::initTasks()
{
    Foliage::BG_Thread &shr_data = world->foliage.bg_thread;

    float tree_range = 600;
    //float grass_range = 60;

    {
        LockGuard lock_qt_trees(shr_data.sm_mutex);
        // insert 500 trees
        for (int i = 0; i<5000; i++)
        {
            float x = (float)(rand()%1000*tree_range)/1000.f-tree_range/2.f;
            float z = (float)(rand()%1000*tree_range)/1000.f-tree_range/2.f;
            glm::vec4 pos = glm::vec4(x, world->terrain.ySample(x, z), z, rand()%360);
            shr_data.qt_trees.insert(FolSpec(pos, FolSpec::Type::Spruce));
        }
//        for (int i = 0; i<1000; i++)
//        {
//            float x = (float)(rand()%1000*grass_range)/1000.f-grass_range/2.f;
//            float z = (float)(rand()%1000*grass_range)/1000.f-grass_range/2.f;
//            glm::vec4 pos = glm::vec4(x, world->terrain.ySample(x, z), z, rand()%360);
//            shr_data.qt_trees.insert(FolSpec(pos, FolSpec::Type::GrassSpring));
//        }
        justFill(glm::vec4(0.0, 0.0, 0.0, 1.0), FolSpec::Type::GrassSpring, 60.f/50.f);
    }
}

void BackGroundMaster::checkAndFill(QuadAABB box, FolSpec::Type type, float scale)
{
    // check all sub-boxes within the box
    // if there is none, then fill that part

    // first find the player's position
//    glm::vec3 ref_position = world->chasecam->pos;


}

void BackGroundMaster::justFill(glm::vec4 center, FolSpec::Type type, float scale)
{
    Foliage::BG_Thread &shr_data = world->foliage.bg_thread;

    for (int i = 0; i<D_PATCH_NUM; i++)
    {
        float x = center.x + discrete_patch[i].x*scale;
        float z = center.z + discrete_patch[i].z*scale;
        glm::vec4 pos = glm::vec4(x, world->terrain.ySample(x, z), z, rand()%360);
        shr_data.qt_trees.insert(FolSpec(pos, type));
    }
}


void BackGroundMaster::mainLoop()
{
    //glm::vec3 const * const someptr = &world->chasecam->pos;

    while (true)
    {
        //updateTasks();
        //test_var+=4.0;

        //float something = 41241;

        //glm::vec3 my_vec = glm::vec3(1.0, 1.0, 1.0);

        checkAndFill(QuadAABB({0.0, 0.0, 0.0, 0.0}), FolSpec::Type::GrassSpring, 5.0);

        ThreadSleep_milli(10); // sleep for 1/100 second
    }
}

