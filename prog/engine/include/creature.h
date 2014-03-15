#ifndef CREATURE_H
#define CREATURE_H

#include "actor.h"
#include "creaturesignals.h"

class Creature : public Actor, virtual public SignalReceiver // count virtula looms in the distance
{
    public:
        Creature();
        virtual ~Creature();

        void resolveActionRequests(float dt);

        // Creature's implementation of MoveSignalReceiver
        void moveForward(float check_sign, float dt);
        void moveLeft(float check_sign, float dt);
        void rotateUp(float unused, float dt);
        void rotateLeft(float check_sign, float dt);

        //void changeStance(int value);
        void attack();
        void dodge();
        void block();
        void shift();


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


        struct State // Could use bitflags for this?
        {
//            bool isIdle;
            bool isShiftDown;
        } state;

        struct Action
        {
            Signal signal;
            float time;
        } doing;

        std::vector<Signal> signal_stack;

        static unsigned int const signal_stack_capacity = 20;
};

#endif // CREATURE_H
