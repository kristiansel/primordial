#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "prop.h"
#include "staticbody.h"

class Obstacle : public Prop, public StaticBody
{
    public:
        Obstacle();
        virtual ~Obstacle();
    protected:
    private:
};

#endif // OBSTACLE_H
