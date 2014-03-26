#ifndef CREATURE_H
#define CREATURE_H

#include "actor.h"
#include "creaturesignals.h"
#include "charactercontroller.h"

class Creature : public Actor, virtual public SignalReceiver// count virtula looms

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

        void jump();

        void stance();


        glm::vec3 getLookDir() const;
        void setLookDir(glm::vec3 u);
//
        glm::quat getLookRot() const;


        void updateTransformation();

        // character physics
        DynamicCharacterController* getCharContr();

    protected:
    private:
        float walkspeed;
        float runspeed;

        // rotation representing look direction (different from 3d object facing direction)
        glm::quat look_rot;


        struct State // Could use bitflags for this?
        {
            bool isShiftDown;
            bool inCombat; // combat or not...
            unsigned char dirflags; // use a bitmask to represent directions
            unsigned char prev_dirflags;
            float up_sweep; // mouse rotation up
            float left_sweep; // mouse rotation left (used for determining attack angle)
            bool moveInterrupted;
            //sSignal prev_signal;
            // temporary for debugging
        } state;

        struct Action
        {
            sSignal signal;
            float time;
            bool triggered;
        } doing;

        std::vector<sSignal> signal_stack;

        static unsigned int const signal_stack_capacity = 20;

        DynamicCharacterController* char_contr;
};

#endif // CREATURE_H
