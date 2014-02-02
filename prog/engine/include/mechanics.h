#ifndef MECHANICS_H
#define MECHANICS_H

#include <sstream>
#include "scene.h"
#include "world.h"

class Mechanics
{
public:
    Mechanics();
    virtual ~Mechanics();

    void init(Scene &scene_in, float &dt_in); /// remove the dt after a while
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
    Scene *scene;
    float* dt; /// refactor away

    const float speed;          /// 1.0/s
    const float camTurnSpeed;   /// deg/s

    /// Temporary
    list<Prop>::iterator prop_it;
};

#endif // MECHANICS_H
