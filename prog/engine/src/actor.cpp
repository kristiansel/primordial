#include "actor.h"


Actor::Actor() : num_pose_matrices(1),
                 pose_matrices(new glm::mat4(1.0)),
                 active_anim(0),
                 active_anim_time(0.f),
                 paused(false),
                 speed_factor(1.0),
                 skel_ptr(nullptr) /// Really should start doing this more....
{
    /// Actors with no attached skeleton will have a default
    /// identity matrix, as not to crash vertex shader
}

Actor::~Actor()
{
    //dtor
    delete [] pose_matrices;
}

void Actor::attachSkeleton(std::weak_ptr<Skeleton> skel_ptr_in)
{
    /// Set the pointer
    skel_ptr = std::shared_ptr<Skeleton>(skel_ptr_in);

    // temporary working shared pointer
    auto shared_skel_ptr = std::shared_ptr<Skeleton>(skel_ptr);

    /// Delete old matrices
    delete [] pose_matrices;

//    /// Set num_pose_matrices
//    num_pose_matrices = shared_skel_ptr->getNumBones();
//
//    /// Initialize the matrices to the same number of bones as the skel
//    pose_matrices = new glm::mat4 [num_pose_matrices];

    /// THREAD SAFE ALTERNATIVE:
    int num_bones = shared_skel_ptr->getNumBones();

    pose_matrices = new glm::mat4 [num_bones];

    num_pose_matrices = num_bones;
}

std::shared_ptr<Skeleton> Actor::shSkelPtr()
{
    return std::shared_ptr<Skeleton>(skel_ptr);
}

void Actor::pose(int anim_index, float time)
{
    /// Checking for null here. Is it performant?

    if (skel_ptr) skel_ptr->poseMatrices(pose_matrices, anim_index, time);
}

void Actor::poseRest()
{
    // fill
    for (int i = 0; i<num_pose_matrices; i++)
    {
        pose_matrices[i] = glm::mat4(1.0);
    }
    paused = true;
}

void Actor::playAnim(int anim_index_in, float speed)
{
    if (anim_index_in < skel_ptr->getNumAnims())
    {
        if (active_anim != anim_index_in) // If not currently playing
        {
            active_anim = anim_index_in; // change to new animation
            active_anim_time = 0.0;
        }

        speed_factor = speed;
        paused = false;
    }
    else
    {
        std::cerr << "anim_index_in > number of animtions" << std::endl;
    }

}

void Actor::updateAnim(float dt)
{
    if (!paused)
    {
        active_anim_time+=speed_factor*dt;
        if (skel_ptr) skel_ptr->poseMatrices(pose_matrices, active_anim, active_anim_time);
    }
}

void Actor::pauseAnim()
{
    paused = true;
}

void Actor::unPauseAnim()
{
    paused = false;
}

void Actor::togglePauseAnim()
{
    paused = !paused;
}

float Actor::getActiveAnimTime() const
{
    return active_anim_time;
}

float Actor::getActiveAnimTimeMod() const
{
    float duration = 0;
    if (skel_ptr) duration = skel_ptr->animations[active_anim].duration;
    return active_anim_time-(int)(active_anim_time/duration)*duration;
}

int Actor::getActiveAnimIndex() const
{
    return active_anim;
}

int Actor::getNumAnims() const
{
    if (skel_ptr) return skel_ptr->getNumAnims();
}
