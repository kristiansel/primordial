#ifndef ACTOR_H
#define ACTOR_H

#include "animatable.h"


class Actor : protected Animatable
{
    public:
        Actor();
        virtual ~Actor();
    protected:
    private:
};

#endif // ACTOR_H
