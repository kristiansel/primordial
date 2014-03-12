#include "creature.h"

Creature::Creature() :
    movespeed(3.0) // m/s (1.5 walk, 3.0 jog, 6.0 run)
{
    //ctor
    signal_stack.reserve(signal_stack_capacity);
}

Creature::~Creature()
{
    //dtor
}

void Creature::resolveActionRequests()
{
    bool moving = false;
//    receive
    while (signal_stack.size() > 0)
    {
        // get top signal
        Signal signal = signal_stack.back();
        signal_stack.pop_back();

        // process signal
        if (signal == Signal::Move) moving = true;
    }

    if (!moving) playAnim(Anim::Idle);
}

/// Animations

void Creature::moveForward(float check_sign, float dt)
{
    // Move
    float sign = (check_sign>0) ? 1.0 : -1.0 ;
    float amount = sign * movespeed * dt;

    Object3d::moveForward(amount, 0.0); // second argument unused;

    // Blend into forward movement animation
    if (check_sign>0)
        playAnim(Anim::Run);
    else
        playAnim(Anim::Run, 0.5);
        //playAnim(Anim::Run, -1.0); // Negative numbers for speed do not work so well...
                                  // should probably look into timeseries for supporting
                                  // backward animation playback

    signal_stack.push_back(Signal::Move);
}

void Creature::moveLeft(float check_sign, float dt)
{
    float sign = (check_sign>0) ? 1.0 : -1.0 ;
    float amount = sign * movespeed * dt;

    // Move
    Object3d::moveLeft(amount, 0.0);

    // Blend into left movement
    // create the animation specifically in blender
    signal_stack.push_back(Signal::Move);
}

void Creature::rotateUp(float unused, float dt_unused)
{
    // Creatures can not be rotated up
}

void Creature::rotateLeft(float check_sign, float dt_unused)
{
    Object3d::rotateLeft(check_sign, dt_unused);
}
