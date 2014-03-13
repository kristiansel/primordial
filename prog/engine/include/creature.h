#ifndef CREATURE_H
#define CREATURE_H

#include "actor.h"

class Creature : public Actor, virtual public MoveSignalReceiver
{
    public:
        Creature();
        virtual ~Creature();

        void resolveActionRequests();

        // Creature's implementation of MoveSignalReceiver
        void moveForward(float check_sign, float dt);
        void moveLeft(float check_sign, float dt);
        void rotateUp(float unused, float dt);
        void rotateLeft(float check_sign, float dt);


        glm::vec3 getLookDir() const;
        void setLookDir(glm::vec3 u);
//
        glm::quat getLookRot() const;
//        void setLookRot(glm::quat q_in);
//
//        void lookInDir(const glm::vec3 &dir_in);
//        void lookAt(const glm::vec3 &pos_in);

    protected:
    private:
        float movespeed;

        // rotation representing look direction (different from 3d object facing direction)
        glm::quat look_rot;


        // signals and animation
        // |
        // |
        // |
        // V
        enum Anim
        {
            Idle = 0,
            Walk = 1,
            Run = 2,
            Idle1H = 3,
            Walk1H = 4,
            StrafeLeft1H = 5,
            StrafeRight1H = 6,
            SwingLeftRight1H = 7,
            SwingRightLeft1H = 8,
            ParryLeft1H = 9,
            ParryRight1H = 10,
            DodgeBack = 11
        };

        enum Signal
        {
            Move = 0
        };

        std::vector<Signal> signal_stack;

        static unsigned int const signal_stack_capacity = 20;
};

#endif // CREATURE_H
