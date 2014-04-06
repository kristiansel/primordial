#ifndef SIGNALS_H
#define SIGNALS_H

#include <iostream>


// Consider making an abstract base class template for customizable
// Signal names "compile time"

struct SignalReceiver
{
    virtual void moveForward(float speed, float dt) = 0;
    virtual void moveLeft(float speed, float dt) = 0;

    virtual void rotateUp(float speed, float dt) = 0;
    virtual void rotateLeft(float speed, float dt) = 0;

    // should make the following methods abstract as well
    virtual void changeStance(int value) {std::cerr << "changeStance signal received, not implemented\n";};
    virtual void attack() {std::cerr << "attack signal received, not implemented\n";};
    virtual void dodge() {std::cerr << "dodge signal received, not implemented\n";};
    virtual void block() {std::cerr << "block signal received, not implemented\n";};
    virtual void shift() {std::cerr << "shift signal received, not implemented\n";};
    virtual void jump() {std::cerr << "jump signal received, not implemented\n";};
    virtual void stance() {std::cerr << "stance signal received, not implemented\n";};

    // Change the of this class to Controllable/Observable
    // observing functions
    virtual bool isAttacking() {std::cerr << "isAttacking observation requested, not implemented\n";};
    virtual bool isBlocking() {std::cerr << "isBlocking observation requested, not implemented\n";};
    virtual bool isInCombat() {std::cerr << "isInCombat observation requested, not implemented\n";};

    // should have something like isDoing(Action::Block) // where Action::Block is a bit flag
};

#endif // SIGNALS_H
