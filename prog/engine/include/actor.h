#ifndef ACTOR_H
#define ACTOR_H

#include "skeleton.h"
#include "prop.h"


class Actor : public Skeleton, public Prop
{
    public:
        Actor();
        virtual ~Actor();
    protected:
    private:
};

#endif // ACTOR_H
