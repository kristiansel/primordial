#ifndef MECHANICS_H
#define MECHANICS_H

#include <sstream>
#include "world.h"

/// For testing
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include "transform.h"

class Mechanics
{
public:
    Mechanics();
    virtual ~Mechanics();

    /// Issue: What should be the division of tasks between
    /// Mechanics and world?

    /// world->addCreature OK
    /// but world->Step(dt) not sure

    /// Input/AI = WorldCreature signals (stored on each creature "ActionRequests")
    /// Resolve these requests (game "mechanics") to find the active "Action"
    /// "Action" determines forces (for physics) and "Animation State"
    /// Forces acted out together with hit testing for Combat
    /// These produce ActionRequests for the next frame, for example if someone was
    /// hit, this takes precedence over their own AI wishes

    void init(World &world_in, float &dt_in); /// remove the dt after a while
    void step(World &world_in, float dt_in);

    string debugInfo();

    /// Controls
    void playerMoveForward();
    void playerMoveBackward();
    void playerMoveLeft();
    void playerMoveRight();

    void playerRotateUp();
    void playerRotateDown();
    void playerRotateRight();
    void playerRotateLeft();

    /// Testing
    void func(int num_in);
protected:
private:
    World *world;
    float* dt; /// refactor away

    const float speed;          /// 1.0/s
    const float camTurnSpeed;   /// deg/s

    /// Temporary
    list<shared_ptr<WorldObject>>::iterator worldobject_ptr_it;
};

#endif // MECHANICS_H
