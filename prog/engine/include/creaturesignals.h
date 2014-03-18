#ifndef CREATURESIGNALS_H
#define CREATURESIGNALS_H

#include <functional>

enum Anim
{
    Idle = 0,
    WalkForward = 1,
    WalkLeft = 2,
    WalkLeftDiag = 3,
    WalkRight = 4,
    WalkRightDiag = 5,
    RunForward = 6,
    RunLeft = 7,
    RunLeftDiag = 8,
    RunRight = 9,
    RunRightDiag = 10,
    Idle1H = 11,
    WalkForward1H = 12,
    StrafeLeft1H = 13,
    StrafeRight1H = 14,
    SwingRight1H = 15,
    SwingLeft1H = 16,
    ParryLeft1H = 17,
    ParryRight1H = 18,
    DodgeBack = 19,
    JumpUp = 20,
    WalkBackward = 21,
    WalkBackLeft = 22,
    WalkBackRight = 23

};

enum sSignal
{
    sDodge = 4,
    sBlock = 3,
    sAttack = 2,
    sJump = 1,
    sMove = 0,
    sNothing = -1
};

namespace dirflag
{
    const unsigned char none = 0;         // 00000000

    const unsigned char forw = 1;         // 00000001
    const unsigned char backw = 2;        // 00000010
    const unsigned char left = 4;         // 00000100
    const unsigned char right = 8;       // 00001000
}

/// forward declaration of creature
//class Creature;
//
//struct Signal
//{
//    //Signal()
//    float amount;
//    int priority;
//    std::function<void(const Creature&)> modifier;
//
//    bool operator < (const Signal &right) // compare priorities
//    {
//        return priority < right.priority;
//    }
//
//    bool operator==(const Signal &right) // compare addresses
//    {
//        return (this==&right);
//    }
//};

//namespace Signal
//{
//    const Dodge
//}


#endif // CREATURESIGNALS_H
