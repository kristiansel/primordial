#include "actor.h"


Actor::Actor() : num_pose_matrices(1),
                 pose_matrices(new glm::mat4(1.0)),
                 active_anim(0),
                 active_anim_time(0.f),
                 paused(false),
                 speed_factor(1.0),
                 skel_ptr(nullptr)
{
    /// Actors with no attached skeleton will have a default
    /// identity matrix, as not to crash vertex shader

    /// Turns out the above comment is very stupid. Actor will get assigned
    /// garbage skeleton if .bbns file is not found.. dangerous stuff

    active_anims.reserve(active_anims_capacity);
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

        // No blending
        if (active_anim != anim_index_in) // If not currently playing
        {
            active_anim = anim_index_in; // change to new animation
            active_anim_time = 0.0;
        }

//        // With blending
//        if (active_anim != anim_index_in) // If not currently the main animation
//        {
//            active_anim = anim_index_in; // change to new animation
//                                    // index        blend_weight    time
//
//            bool found = false;
//            for (ActiveAnim &anim : active_anims)
//            {
//                if (anim.anim_index == anim_index_in)
//                    found = true;
//            }
//            if (!found)
//                active_anims.push_back( {anim_index_in, 0.0,            0.0} );
//        }

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
        // No blending
        active_anim_time+=speed_factor*dt;
        if (skel_ptr) skel_ptr->poseMatrices(pose_matrices, active_anim, active_anim_time);

//        // With blending
//        for(ActiveAnim &anim : active_anims)
//        {
//            float advance_time = speed_factor*dt;
//            anim.anim_time+=advance_time;
//
//            float weight_change = advance_time/blend_time;
//            if (anim.anim_index == active_anim) // It is the main active animation
//            {
//                // increase blend weight
//                float new_weigth = anim.blend_weight + weight_change;
//
//                // 1.0 is the maximum
//                anim.blend_weight = (1.0 < new_weight) ? 1.0 : new_weight;
//            }
//            else // It is not the main active animation
//            {
//                // decrease the blend weight
//                anim.blend_weight = anim.blend.weight - weight_change;
//
//                // let it go below zero to mark animations that can be
//                // taken out of active_anims
//            }
//        }
//        // remove if blend weight below 0.0
//        active_anims.erase( std::remove_if(active_anims.begin(),
//                                           active_anims.begin(),
//                                           [] (ActiveAnim &a) {return (a.blend_weight < 0.0);}));
//        // who invented this syntax...


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
