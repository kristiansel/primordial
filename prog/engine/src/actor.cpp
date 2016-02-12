#include "actor.h"


Actor::Actor() : skel_ptr(nullptr),
                 num_pose_matrices(1),
                 pose_matrices(new glm::mat4(1.0)),
                 active_anim(-1),
                 main_anim_uid(-1),
                 m_stagger_time(0.0),
                 paused(true),
                 speed_factor(1.0)
{
    // Actors with no attached skeleton will have a default
    // identity matrix, as not to crash vertex shader

    // Turns out the above comment is very stupid. Actor will get assigned
    // garbage skeleton if .bbns file is not found.. dangerous stuff

    active_anims.reserve(active_anims_capacity);
}

Actor::~Actor()
{
    //dtor
    delete [] pose_matrices;
}

void Actor::attachSkeleton(std::weak_ptr<Skeleton> skel_ptr_in)
{
    // Set the pointer
    skel_ptr = std::shared_ptr<Skeleton>(skel_ptr_in);

    // temporary working shared pointer
    auto shared_skel_ptr = std::shared_ptr<Skeleton>(skel_ptr);

    // Delete old matrices
    delete [] pose_matrices;

//    // Set num_pose_matrices
//    num_pose_matrices = shared_skel_ptr->getNumBones();
//
//    // Initialize the matrices to the same number of bones as the skel
//    pose_matrices = new glm::mat4 [num_pose_matrices];

    // THREAD SAFE ALTERNATIVE:
    int num_bones = shared_skel_ptr->getNumBones();

    pose_matrices = new glm::mat4 [num_bones];

    num_pose_matrices = num_bones;
}

std::shared_ptr<Skeleton> Actor::shSkelPtr()
{
    return std::shared_ptr<Skeleton>(skel_ptr);
}

//void Actor::pose(int anim_index, float time)
//{
//    // Checking for null here. Is it performant?
//
//    if (skel_ptr) skel_ptr->poseMatrices(pose_matrices, anim_index, time);
//}

void Actor::poseRest()
{
    // fill
    for (int i = 0; i<num_pose_matrices; i++)
    {
        pose_matrices[i] = glm::mat4(1.0);
    }
    paused = true;
}

void Actor::playAnim(int anim_index_in,
                     bool restart,          // If already the main one, blend with self
//                     bool sync,             // Syncronise time with current animation
                     float speed)           // set speed (does not support negative speed yet)
{
    if (anim_index_in < skel_ptr->getNumAnims() && !(anim_index_in<0))
    {
        active_anim = anim_index_in; // change to new animation

        // If not restart, then check if the animation
        // is already playing

        bool found = false;

        if (!restart)
        {
            for (ActiveAnim &anim : active_anims)
            {
                if (anim.anim_index == anim_index_in)
                {
                    found = true; // if it is already playing
                    main_anim_uid = anim.uid; // make it the main
                }
            }
        }

        if (!found) // if not currently playing
        {
            // Start the animation

            // generate a unique ID for this clip
            int uid = uid_gen.getClipUID();

            // If restarting
            float start_time = 0.0;

            // If not, get the time of the currently main animation
            if (!restart && !active_anims.empty())
            {
                // find element with max blend weight
                auto main_anim = std::max_element(active_anims.begin(), active_anims.end() );

                start_time = main_anim->anim_time;
            }

            // add it to the list of blend animations
            active_anims.push_back( {anim_index_in, // animation index
                                     0.0,           // initial blend weight
                                     start_time,    // initial time
                                     uid} );        // unique clip id

            // Set main clip
            main_anim_uid = uid;

            // Set speed on animation start
            speed_factor = speed;
        }

        // Could set speed every time playAnim is called and allow for animations to
        // change speed under playing, however, knowing beforehand how
        // long an animation will last simplifies the animation-transition
        // flow logic

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

        // With blending
        for(ActiveAnim &anim : active_anims)
        {
            float advance_time = speed_factor*dt;
            anim.anim_time+=advance_time;

            float weight_change = advance_time/blend_time; // 1.0 * fraction of blend_time

            if (anim.uid == main_anim_uid) // It is the main active animation
            {
                // increase blend weight
                float new_weight = anim.blend_weight + weight_change;

                // 1.0 is the maximum
                anim.blend_weight = (1.0 < new_weight) ? 1.0 : new_weight;
            }
            else // It is not the main active animation
            {
                // decrease the blend weight
                anim.blend_weight = anim.blend_weight - weight_change;

                // let it go below zero to mark animations that can be
                // taken out of active_anims
            }
        }

        // remove if blend weight below 0.0
        auto has_neg_weight = [] (ActiveAnim &a) {return (a.blend_weight < 0.0);};

        if (!active_anims.empty())
            active_anims.erase(
                std::remove_if(active_anims.begin(), active_anims.end(), has_neg_weight),
                active_anims.end());

        // pose matrices:
        if (skel_ptr) skel_ptr->poseMatricesBlend(pose_matrices, active_anims);

        // pose slots:
        // Pose parented render-batches (DEFINITLY REVISE THIS)
        for (auto &render_batch : render_batches)
        {
            int parent_index = render_batch.parent_bone;

            if (parent_index != -1)
            {
                if (parent_index<num_pose_matrices && parent_index>=0)
                {
                    // int parent = render_batch.parent_bone;

                    // This code is a work-around (the price of not having a proper scene graph)
                    glm::mat4 id(1.0);
                    glm::mat4 rot = glm::rotate(id, (float)(-3.14/2.0), glm::vec3(1.f, 0.f, 0.f));
                    glm::mat4 tra = glm::translate(glm::mat4(1.0), glm::vec3(0.74104, 1.33846, -0.06084));

                    // tra and rot are the translation and rotation of the slot...
                    render_batch.transf_mat = pose_matrices[parent_index] * tra * rot * glm::inverse(pose_matrices[0]);
                    // as for why we post multiply by inverse of pose_matrices[0], that is because in the vertex shader
                    // models that are not "skinned" use bone number 0 for transformation, because I am too lazy to write
                    // a shader for non-skinned meshes...
                }
                else
                {
                    std::cerr << "error: assigned slot parent bone out of bounds\n";
                }
            }

        }

//        std:cout << "length of active_anims: " << active_anims.size() << "\n";
    } // if not paused
    else
    {
        // if paused it might be staggered
        if (m_stagger_time > 0.000001) // this means it has been staggered
        {
            float advance_time = speed_factor*dt;
            m_stagger_time -=advance_time;

            if (m_stagger_time <= 0.000001)
            {
                // resume playing animation
                unPauseAnim();
            }
        }
    }
}

bool Actor::poseMatrices(glm::mat4* mat_ptr)
{
    skel_ptr->poseMatricesBlend(mat_ptr, active_anims);
    return true;
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

void Actor::staggerAnim(float stagger_time)
{
    pauseAnim();
    m_stagger_time = stagger_time;
}

//float Actor::getActiveAnimTime() const
//{
//    return active_anim_time;
//}

//float Actor::getActiveAnimTimeMod() const
//{
//    float duration = 0;
//    if (skel_ptr) duration = skel_ptr->animations[active_anim].duration;
//    return active_anim_time-(int)(active_anim_time/duration)*duration;
//}

int Actor::getActiveAnimIndex() const
{
    return active_anim;
}

int Actor::getNumAnims() const
{
    return skel_ptr ? skel_ptr->getNumAnims() : 0;
}

float Actor::getAnimDuration(int anim_index)
{
    return ( (skel_ptr) ? skel_ptr->getAnimDuration(anim_index) : 0.0 );
}

void Actor::moveBatchToSlot(RenderBatch* batch, Slot slot_index)
{
    // do
    if (slot_index < skel_ptr->num_slots && slot_index>=0)
    {
        batch->parent_bone = skel_ptr->slots[slot_index].parent_bone_index;
    }
    else
    {
        std::cerr << "error: attempted to assign to slot out of bounds\n";
    }
}
