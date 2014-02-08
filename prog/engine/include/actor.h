#ifndef ACTOR_H
#define ACTOR_H

#include "skeleton.h"
#include "prop.h"


class Actor : public Skeleton, public Prop
{
    public:
        Actor();
        virtual ~Actor();

        /// Add some high level animation functionality later
        /// for now use Skeleton::startAnimation(Anum enum)

    protected:
    private:

};

#endif // ACTOR_H
