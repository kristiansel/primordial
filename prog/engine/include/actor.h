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

        std::weak_ptr<Skeleton> skel_ptr;

        /// could consider getter method/private for:
        int num_pose_matrices;
        glm::mat4* pose_matrices;

    protected:
    private:

};

#endif // ACTOR_H
