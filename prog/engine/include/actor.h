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

        /// Add some high level animation functionality later
        /// for now use Skeleton::startAnimation(Anum enum)

        void attachSkeleton(std::weak_ptr<Skeleton> skel_ptr_in);

        /// Temporary for debugging:
        std::shared_ptr<Skeleton> shSkelPtr();

        std::shared_ptr<Skeleton> skel_ptr;

        void pose(int anim_index, float time);
        void poseRest();
        void playAnim(int anim_index, float speed = 1.0);


        void updateAnim(float dt); /// This is where the calculations are called
        void pauseAnim();
        void unPauseAnim();
        void togglePauseAnim();

        /// debug methods;
        float getActiveAnimTime() const;
        float getActiveAnimTimeMod()  const;
        int getActiveAnimIndex() const;
        int getNumAnims() const;

        /// could consider getter method/private for:
        int num_pose_matrices;
        glm::mat4* pose_matrices;

    protected:
    private:
        int active_anim;
        float active_anim_time;
        bool paused;
        float speed_factor;

        static unsigned int const active_anims_capacity = 10;
        static constexpr float blend_time = 0.200; // 0.2 seconds to go from blend weight 1.0 to blend weight 0.0

        std::vector<ActiveAnim> active_anims;

};

#endif // ACTOR_H
