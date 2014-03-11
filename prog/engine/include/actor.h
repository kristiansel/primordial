#ifndef ACTOR_H
#define ACTOR_H

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

};

#endif // ACTOR_H
