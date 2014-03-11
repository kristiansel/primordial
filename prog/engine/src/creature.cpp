#include "creature.h"

Creature::Creature() :
    movespeed(1.5) // m/s (1.5 walk, 3.0 jog, 6.0 run)
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

void Creature::moveForward(float check_sign)
{
    // Move
    Object3d::moveForward(check_sign);

    // Blend into forward movement animation
    playAnim(Ani::Run);
}

void Creature::moveLeft(float check_sign)
{
    // Move
    Object3d::moveLeft(check_sign);

    // Blend into left movement
}

void Creature::rotateUp(float unused)
{
    // Creatures can not be rotated up
}

void Creature::rotateLeft(float check_sign)
{
    Object3d::rotateLeft(check_sign);
}
