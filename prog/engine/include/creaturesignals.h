#ifndef CREATURESIGNALS_H
#define CREATURESIGNALS_H

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
    JumpUp = 20

};

enum Signal
{
    Dodge = 4,
    Block = 3,
    Attack = 2,
    Jump = 1,
    Move = 0,
    Nothing = -1
};


#endif // CREATURESIGNALS_H
