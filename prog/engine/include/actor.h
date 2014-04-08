#ifndef ACTOR_H
#define ACTOR_H

#include <vector>

#include "skeleton.h"
#include "prop.h"


class Actor : public Prop
{
    public:
        Actor();
        virtual ~Actor();

        // Add some high level animation functionality later
        // for now use Skeleton::startAnimation(Anum enum)

        void attachSkeleton(std::weak_ptr<Skeleton> skel_ptr_in);

        // Temporary for debugging:
        std::shared_ptr<Skeleton> shSkelPtr();

        std::shared_ptr<Skeleton> skel_ptr;

        void pose(int anim_index, float time);
        void poseRest();
        void playAnim(int anim_index,
                      bool restart = false,
                      float speed = 1.0);

        void updateAnim(float dt); // This is where the calculations are called
        void pauseAnim();
        void unPauseAnim();
        void togglePauseAnim();
        float getAnimDuration(int anim_index);
        void staggerAnim(float stagger_time);

        // debug methods;
        int getActiveAnimIndex() const;
        int getNumAnims() const;

        // could consider getter method/private for:
        int num_pose_matrices;
        glm::mat4* pose_matrices;

        // Used by creature to determine action duration
        static constexpr float blend_time = 0.20; // 0.20 seconds to go from blend weight 1.0 to blend weight 0.0

        // make a renderbatch the child of a skeleton slot:
        enum Slot {RightHand = 0};
        void moveBatchToSlot(RenderBatch* batch, Slot slot);

    protected:
    private:
        int active_anim; // The index of the main active animation
        int main_anim_uid; // The index of the main active animation blend clip

        float m_stagger_time; // used for temporarily pausing animation

        bool paused; // is animation turned paused
        float speed_factor; // dt_animation = dt_real * speed_factor

        static unsigned int const active_anims_capacity = 10; // Max number of blending anims


        // list of active animations (currently blending):
        // To blend different animations for different parts of the skeleton
        // could make bone groups and make an array active_anims[MAX_BONE_GROUPS]
        // i.e torso swings sword, while legs run, while head looks at something
        std::vector<ActiveAnim> active_anims;

        // The animation clip UID is needed to
        // distinguish animations with the same animation
        // index, i.e. -> enable blending an animation with itself
        // Of course blending together different times
        struct ClipUIDgenerator
        {
            int uid; // no need to initialize (do not care about value)
            int getClipUID()
            {
                // increment val and return it
                uid++;
                return uid;
            };
        } uid_gen;

};

#endif // ACTOR_H
