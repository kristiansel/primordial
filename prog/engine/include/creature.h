#ifndef CREATURE_H
#define CREATURE_H

#include "actor.h"
#include "creaturesignals.h"
#include "charactercontroller.h"
#include "soundemitter.h"
#include "stats.h"
#include "artificialintelligence.h"
#include <array>

class Creature : public Actor, virtual public SignalReceiver // count virtula looms

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

        // observer functions
        bool isAttacking();
        bool isInCombat();
        bool isBlocking();
        bool isDodging();

        glm::vec3 getLookDir() const;
        void setLookDir(glm::vec3 u);
//
        glm::quat getLookRot() const;

        void updateTransformation();

        void connectAI(ai::Agent* aiAgent, ai::World* aiWorld);

        // character physics
        DynamicCharacterController* getCharContr();
//        void notifyGotHit();

        float getHealth() const;
        void setHealth(float health);

        Stats getStats() const;

    protected:
    private:
        static const unsigned int SIG_REG_SIZE = 5;

        float walkspeed;
        float runspeed;

        // internal signals
        struct HitInfo
        {
            Creature* hitBy; // the creature that did the hitting
            float from_angle; // 0 means top down chop, - from left side, + from right side
            glm::vec3 dir; // the "force" direction of the hit
        };
        void hit(HitInfo hit_info);
        void hitWasBlocked();


        void signal_push(sSignal signal); // push without using registers

        typedef std::array<float, SIG_REG_SIZE> SignArgType;
        void signal_push(sSignal signal, SignArgType args); // push signal and arguments

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
            float time; // used only for action, not animation
            bool triggered;
            std::array<float, SIG_REG_SIZE> reg;
//            float reg[SIG_REG_SIZE]; // provide three registers for signals to store their arguments
            // like direction, magnitude etc...
        } doing;

        struct Candidate    // a place to store signal requests as they are received
        {
            sSignal signal;
            std::array<float, SIG_REG_SIZE> reg;
//            float reg[SIG_REG_SIZE];
        } candidate;

        // stats
        Stats stats;

//        std::vector<sSignal> signal_stack;
//
//        static unsigned int const signal_stack_capacity = 20;

        // "Components" (most of connection functionality is here)
        DynamicCharacterController* char_contr;
        SoundEmitter* snd_emitter;

        const Creature * target_creature;

        ai::Agent *m_aiAgent;
        ai::World *m_aiWorld;


};

#endif // CREATURE_H
