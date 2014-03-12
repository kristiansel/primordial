#ifndef CREATURE_H
#define CREATURE_H

#include "actor.h"

class Creature : public Actor, virtual public MoveSignalReceiver
{
    public:
        Creature();
        virtual ~Creature();

        // Creature's implementation of MoveSignalReceiver
        void moveForward(float check_sign, float dt);
        void moveLeft(float check_sign, float dt);
        void rotateUp(float unused, float dt);
        void rotateLeft(float check_sign, float dt);
    protected:
    private:
        float movespeed;

};

#endif // CREATURE_H
