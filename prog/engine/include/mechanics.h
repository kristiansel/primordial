#ifndef MECHANICS_H
#define MECHANICS_H

#include <sstream>
#include "scene.h"

class Mechanics
{
public:
    Mechanics();
    virtual ~Mechanics();

    void init(Scene &scene_in, float &dt_in); /// remove the dt after a while

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
protected:
private:
    Scene *scene;
    float* dt; /// refactor away

    const float speed;          /// 1.0/s
    const float camTurnSpeed;   /// deg/s
};

#endif // MECHANICS_H
