#ifndef CREATURE_H
#define CREATURE_H

#include "actor.h"

class Creature : public Actor, virtual public MoveSignalReceiver
{
    public:
        Creature();
        virtual ~Creature();

        // Creature's implementation of MoveSignalReceiver
        void moveForward(float check_sign);
        void moveLeft(float check_sign);
        void rotateUp(float unused);
        void rotateLeft(float check_sign);
    protected:
    private:
        float movespeed;

};

#endif // CREATURE_H
