#ifndef MECHANICS_H
#define MECHANICS_H

#include <sstream>
#include "world.h"

class Mechanics
{
public:
    Mechanics();
    virtual ~Mechanics();

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
    list<shared_ptr<Obstacle>>::iterator obstacle_ptr_it;
};

#endif // MECHANICS_H
