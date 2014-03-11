#ifndef SIGNALS_H
#define SIGNALS_H


/// Consider making an abstract base class template for customizable
/// Signal names "compile time"

struct MoveSignalReceiver
{
    virtual void moveForward(float gain) = 0;
    virtual void moveLeft(float gain) = 0;

    virtual void rotateUp(float gain) = 0;
    virtual void rotateLeft(float gain) = 0;
};

#endif // SIGNALS_H
