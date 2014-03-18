#include "creature.h"

Creature::Creature() :
    walkspeed(1.5), // m/s
    runspeed(3.0),
    //look_rot(glm::quat(1.0, 0.0, 0.0, 0.0))
    look_rot(rot),
    // isIdle, isShiftDown
    state( {false} ),
    doing( {sSignal::sNothing, 0.000})
{
    //ctor
    signal_stack.reserve(signal_stack_capacity);
}

Creature::~Creature()
{
    //dtor
}

void Creature::resolveActionRequests(float dt)
{
    // decrement the time
    doing.time -=dt;
    if (!signal_stack.empty())
    {
        std::sort(signal_stack.begin(), signal_stack.end());

        // get reference to top prioritized signal
        sSignal &top_pri = signal_stack.back();

        // Gameplay decision: If singals should be hierarchical (uncomment)
        // or the new signal should always interupt current (comment)
        // if (top_pri > doing.signal)

        // execute that signal
        switch (top_pri)
        {
        case sSignal::sMove: // most likely
            {
                doing = {sMove, 1.0/20.0}; // keep moving for a twentieth of a second

                // choose correct move animation based on the combination of direction
                // signals received

                // 00001111, 00000000, 00001100, 00000011 -> idle   15, 0, 12, 3
                // 00000101 -> diagonal left                        5
                // 00001001 -> diagonal right                       9
                // 00000110 -> diagonal back left                   6
                // 00001010 -> diagonal back right                  10
                // 00000001, 00001101 -> forward                    1, 13
                // 00000010, 00001110 -> backward                   2, 14
                // 00000100, 00000111 -> left                       4, 7
                // 00001000, 00001011 -> right                      8, 11

                // if shift -> run
                if (state.dirflags==1 || state.dirflags==13)    // forward
                {
                    if (state.isShiftDown)
                    {
                        playAnim(Anim::RunForward);
                        Object3d::moveForward(runspeed*dt, 0.0);
                    }
                    else
                    {
                        playAnim(Anim::WalkForward);
                        Object3d::moveForward(walkspeed*dt, 0.0);
                    }
                }
                if (state.dirflags==4 || state.dirflags==7)     // left
                {
                    if (state.isShiftDown)
                    {
                        playAnim(Anim::RunLeft);
                        Object3d::moveLeft(runspeed*dt, 0.0);
                    }
                    else
                    {
                        playAnim(Anim::WalkLeft);
                        Object3d::moveLeft(walkspeed*dt, 0.0);
                    }
                }
                if (state.dirflags==8 || state.dirflags==11)    // right
                {
                    if (state.isShiftDown)
                    {
                        playAnim(Anim::RunRight);
                        Object3d::moveLeft(-runspeed*dt, 0.0);
                    }
                    else
                    {
                        playAnim(Anim::WalkRight);
                        Object3d::moveLeft(-walkspeed*dt, 0.0);
                    }
                }
                if (state.dirflags==5)                          // left diag
                {
                    if (state.isShiftDown)
                    {
                        playAnim(Anim::RunLeftDiag);
                        Object3d::moveLeft(0.70710678*runspeed*dt, 0.0);
                        Object3d::moveForward(0.70710678*runspeed*dt, 0.0);
                    }
                    else
                    {
                        playAnim(Anim::WalkLeftDiag);
                        Object3d::moveLeft(0.70710678*walkspeed*dt, 0.0);
                        Object3d::moveForward(0.70710678*walkspeed*dt, 0.0);
                    }
                }
                if (state.dirflags==9)                          // right diag
                {
                    if (state.isShiftDown)
                    {
                        playAnim(Anim::RunRightDiag);
                        Object3d::moveLeft(-0.70710678*runspeed*dt, 0.0);
                        Object3d::moveForward(0.70710678*runspeed*dt, 0.0);
                    }
                    else
                    {
                        playAnim(Anim::WalkRightDiag);
                        Object3d::moveLeft(-0.70710678*walkspeed*dt, 0.0);
                        Object3d::moveForward(0.70710678*walkspeed*dt, 0.0);
                    }
                }
                if (state.dirflags==2 || state.dirflags==14)                          // backwards
                {
                    // cannot run backwards
                    playAnim(Anim::WalkBackward);
                    Object3d::moveForward(-walkspeed*dt, 0.0);
                }
                if (state.dirflags==6)                          // back left
                {
                    // cannot run backwards
                    playAnim(Anim::WalkBackLeft);
                    Object3d::moveForward(-0.70710678*walkspeed*dt, 0.0);
                    Object3d::moveLeft(0.70710678*walkspeed*dt, 0.0);
                }
                if (state.dirflags==10)                          // back right
                {
                    // cannot run backwards
                    playAnim(Anim::WalkBackRight);
                    Object3d::moveForward(-0.70710678*walkspeed*dt, 0.0);
                    Object3d::moveLeft(-0.70710678*walkspeed*dt, 0.0);
                }

            } break;
        case sSignal::sAttack:
            {
                float speed = 1.0;

                doing = {sAttack, getAnimDuration(Anim::SwingRight1H)/speed};

                playAnim(Anim::SwingRight1H, true, speed);
            } break;
        case sSignal::sDodge:
            {
                float speed = 1.0;

                doing = {sDodge, getAnimDuration(Anim::DodgeBack)/speed};

                playAnim(Anim::DodgeBack, true, speed);
            } break;
        case sSignal::sBlock:
            {
                float speed = 1.0;

                doing = {sBlock, getAnimDuration(Anim::ParryLeft1H)/speed};

                playAnim(Anim::ParryLeft1H, true, speed);
            } break;
        case sSignal::sJump:
            {
                float speed = 1.0;

                doing = {sJump, getAnimDuration(Anim::JumpUp)/speed};

                playAnim(Anim::JumpUp, true, speed);
            } break;
        }
    }
    else // No signal received
    {
        // Start blending into idle when only "blend time" remains
        // if we set 0.0 here, blend-to-idle will start at 0.0, and
        // the old animation will start replaying (for "blend time"
        // more seconds, this does not look good, so we start blending
        // into idle while there is still some time remaining on the old
        // animation
        if (doing.time < Actor::blend_time) // finished whatever was doing
        {
            playAnim(Anim::Idle); // pre-emptively blend into idle

            if (doing.time < 0.0) // but the ACTION is not finished until time 0.0
            {
                // Then do nothing
                doing = {sNothing, 0.000};
            }
        }
        // else keep on playing whatever animation was playing
    }

    // Get ready for next frame

    // clear signal stack
    signal_stack.clear();

    // shift is not held down generally
    state.isShiftDown = false;

    // clear bitflags
    state.dirflags = dirflag::none;
}

// Animations

void Creature::moveForward(float check_sign, float dt)
{
    // Move
//    float sign = (check_sign>0) ? 1.0 : -1.0 ;
//    float amount = sign * runspeed * dt;
//
//    Object3d::moveForward(amount, 0.0); // second argument unused;

    if (check_sign < 0)
        state.dirflags = state.dirflags | dirflag::backw;
    else
        state.dirflags = state.dirflags | dirflag::forw;

    signal_stack.push_back(sSignal::sMove);
//    setDir(look_dir_projected_xz);
}

void Creature::moveLeft(float check_sign, float dt)
{
//    float sign = (check_sign>0) ? 1.0 : -1.0 ;
//    float amount = sign * runspeed * dt;
//
//    // Move
//    Object3d::moveLeft(amount, 0.0);

    if (check_sign < 0)
        state.dirflags = state.dirflags | dirflag::right;
    else
        state.dirflags = state.dirflags | dirflag::left;

    // Blend into left movement
    // create the animation specifically in blender
    signal_stack.push_back(sSignal::sMove);
}

void Creature::rotateUp(float degrees, float dt_unused)
{
    // Creatures can not be rotated up
    look_rot = glm::rotate(look_rot, 3.14159265f*degrees/180.f, glm::vec3(1.0, 0.0, 0.0));
}

void Creature::rotateLeft(float degrees, float dt_unused)
{
    // Object3d::rotateLeft(check_sign, dt_unused);


    glm::vec3 axis = glm::inverse(glm::mat3_cast(look_rot)) * glm::vec3(0.0, 1.0, 0.0);
    look_rot = glm::rotate(look_rot, 3.14159265f*degrees/180.f, axis);

    // rotation equals look_rot decomposed to y rotation
    glm::vec3 look_dir = getLookDir();
    glm::vec3 look_dir_projected_xz = glm::normalize(glm::vec3(look_dir.x, 0.f, look_dir.z));
    setDir(look_dir_projected_xz);
}
//
glm::vec3 Creature::getLookDir() const
{
    return glm::mat3_cast(look_rot) * glm::vec3(0.0, 0.0, -1.0);
}

glm::quat Creature::getLookRot() const
{
    return look_rot;
}

void Creature::attack()
{
    signal_stack.push_back(sSignal::sAttack);
}

void Creature::dodge()
{
    signal_stack.push_back(sSignal::sDodge);
}

void Creature::block()
{
    signal_stack.push_back(sSignal::sBlock);
}

void Creature::shift()
{
    state.isShiftDown = true;
}

void Creature::jump()
{
    signal_stack.push_back(sSignal::sJump);
}
