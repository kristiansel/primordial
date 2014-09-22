#ifndef BACKGROUNDMASTER_H
#define BACKGROUNDMASTER_H

//#include "threadiobuffers.hpp"
#include "world.h"

//class BackGroundJob
//{
//    int job_ID;
//    enum class JobType {Job1, Job2}; // etc
//    JobType job_type;
//    int job_flags; // 32 flag bits to set;
//
//    static const unsigned int BG_JOB_SIZE = 52; // bytes
//
//    char job_data[BG_JOB_SIZE];
//
//    // should be total 64 bytes
//
//};

class BackGroundMaster
{
    public:
        //BackGroundMaster(ThreadIObuffers<char, 200>::IOBuffer<1> in);
        BackGroundMaster(World *world_in);
        virtual ~BackGroundMaster();

        void initTasks();
        void mainLoop();

        void wrapUp();
        bool finished();

    protected:
        void checkAndFill(QuadAABB box, FolSpec::Type, float scale);
        void justFill(glm::vec4 center, FolSpec::Type, float scale);
    private:
        World* world;

        sf::Clock clock;

        float test_var;

        // some stamp/brush data
        static const int D_PATCH_NUM = 1000;
        static constexpr float D_PATCH_SCALE = 50.0; // 1000/(50*50) patch = 2 trees for each 5 square mtrs
        glm::vec4 discrete_patch[D_PATCH_NUM];

        bool exit_received;
        bool is_finished;
};

#endif // BACKGROUNDMASTER_H
