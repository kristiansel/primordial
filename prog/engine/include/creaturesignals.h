#ifndef CREATURESIGNALS_H
#define CREATURESIGNALS_H

// signals and animation
// |
// |
// |
// V
enum Anim
{
    Idle = 0,
    Walk = 1,
    Run = 2,
    Idle1H = 3,
    Walk1H = 4,
    StrafeLeft1H = 5,
    StrafeRight1H = 6,
    SwingLeftRight1H = 7,
    SwingRightLeft1H = 8,
    ParryLeft1H = 9,
    ParryRight1H = 10,
    DodgeBack = 11
};

enum Possibility
{
    None = -1,
    Maybe = 0,
    Yes = 1,
};

//enum Signal // Listed by priority
//{
//    Move = 0,
//    Attack = 1,
//    Dodge = 2,
//    Block = 3,
//    Shift = 4
//};

enum Signal
{
    Dodge = 3,
    Block = 2,
    Attack = 1,
    Move = 0,
    Nothing = -1 // basically, always receive Idle signal
};


////enum TypeEnum
////{
////    Move = 0,
////    Attack = 1,
////    Dodge = 2,
////    Block = 3,
////    Shift = 4
////};
//
//struct CreatureSignal
//{
//public:
//
//private:
////    TypeEnum type;
//};

#endif // CREATURESIGNALS_H
