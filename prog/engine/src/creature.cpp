#include "creature.h"

Creature::Creature() :
    movespeed(3.0) // m/s (1.5 walk, 3.0 jog, 6.0 run)
{
    //ctor
}

Creature::~Creature()
{
    //dtor
}

/// Animations
namespace Ani
{
    enum Action { Idle = 0,
                  Walk = 1,
                  Run = 2,
                };
}

void Creature::moveForward(float check_sign, float dt)
{
    // Move
    float sign = (check_sign>0) ? 1.0 : -1.0 ;
    float amount = sign * movespeed * dt;

    Object3d::moveForward(amount, 0.0); // second argument unused;

    // Blend into forward movement animation
    playAnim(Ani::Run);
}

void Creature::moveLeft(float check_sign, float dt)
{
    // Move
    Object3d::moveLeft(check_sign, dt);

    // Blend into left movement
}

void Creature::rotateUp(float unused, float dt_unused)
{
    // Creatures can not be rotated up
}

void Creature::rotateLeft(float check_sign, float dt_unused)
{
    Object3d::rotateLeft(check_sign, dt_unused);
}
