#ifndef MECHANICS_H
#define MECHANICS_H

#include <sstream>
#include "world.h"

// For testing
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "signals.h"
#include "global.h" // temporary for direct loading of files

class Mechanics
{
public:
    Mechanics();
    virtual ~Mechanics();

    // Issue: What should be the division of tasks between
    // Mechanics and world?

    // world->addCreature OK
    // but world->Step(dt) not sure

    // Input/AI = WorldCreature signals (stored on each creature "ActionRequests")
    // Resolve these requests (game "mechanics") to find the active "Action"
    // "Action" determines forces (for physics) and "Animation State"
    // Forces acted out together with hit testing for Combat
    // These produce ActionRequests for the next frame, for example if someone was
    // hit, this takes precedence over their own AI wishes

    void init(World &world_in, float &dt_in); // remove the dt after a while
    void step(World &world_in, float dt_in);

    string debugInfo();

    // External controls
    void playerMoveForward();
    void playerMoveBackward();
    void playerMoveLeft();
    void playerMoveRight();

    void playerRotateUp();
    void playerRotateDown();
    void playerRotateRight();
    void playerRotateLeft();

    void playerRotateUpVal(float);
    void playerRotateLeftVal(float);

    // More controls
    //virtual void playerChangeStance();
    void playerAttack();
    void playerDodge();
    void playerBlock();
    void playerShift();

    void playerJump();

    void playerStance();

    float getInterfaceInfo();

    // Testing
    void func(int num_in);
protected:
    void toggleCamera();
private:
    World *world;
    float* dt; // refactor away

    // Controls related
    const float speed;          // 1.0/s
    const float camTurnSpeed;   // deg/s
    SignalReceiver* controlled;
    Creature * player;    // the creature chased by chase-cam


    // Temporary for testing deletion
    list<shared_ptr<WorldObject>>::iterator worldobject_ptr_it;


    // some temporary creature assembler function
    void addNPC(glm::vec3 pos_in);
};

#endif // MECHANICS_H
