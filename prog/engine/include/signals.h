#ifndef SIGNALS_H
#define SIGNALS_H


/// Consider making an abstract base class template for customizable
/// Signal names "compile time"

struct MoveSignalReceiver
{
    virtual void moveForward(float speed, float dt) = 0;
    virtual void moveLeft(float speed, float dt) = 0;

    virtual void rotateUp(float speed, float dt) = 0;
    virtual void rotateLeft(float speed, float dt) = 0;
};

#endif // SIGNALS_H
