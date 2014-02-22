#include "actor.h"


Actor::Actor() : num_pose_matrices(1),
                 pose_matrices(new glm::mat4(1.0)),
                 active_anim(0),
                 active_anim_time(0.f)
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

    /// Set num_pose_matrices
    num_pose_matrices = shared_skel_ptr->getNumBones();

    /// Initialize the matrices to the same number of bones as the skel
    pose_matrices = new glm::mat4 [num_pose_matrices];
}

std::shared_ptr<Skeleton> Actor::shSkelPtr()
{
    return std::shared_ptr<Skeleton>(skel_ptr);
}

void Actor::pose(int anim_index, float time)
{
    skel_ptr->poseMatrices(pose_matrices, anim_index, time);
}

void Actor::playAnim(int anim_index_in)
{
    active_anim = anim_index_in;
    active_anim_time = 0.0;

}

void Actor::updateAnim(float dt)
{
    active_anim_time+=dt;
    skel_ptr->poseMatrices(pose_matrices, active_anim, active_anim_time);
}
