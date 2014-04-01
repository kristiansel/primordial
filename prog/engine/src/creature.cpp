#include "creature.h"

Creature::Creature() :
    walkspeed(1.5), // m/s
    runspeed(3.0),
    look_rot(rot),
    //              moveInterrupt prev_signal
    state( {false,              //isShiftDown
            false,              //inCombat
            dirflag::none,      //dirflags
            dirflag::none,      //prev_dirflags
            0.f,                //upsw
            0.f,                //leftsw
            false//,              //moveInterrupted
            //sSignal::sNothing   //
            } ),
    doing( {sSignal::sNothing, 0.000, false}),
    char_contr(new DynamicCharacterController),
    snd_emitter(new SoundEmitter)
{
    //ctor
    signal_stack.reserve(signal_stack_capacity);
}

Creature::~Creature()
{
    //dtor
    char_contr->destroy();
    delete char_contr;
    delete snd_emitter;
}

void Creature::resolveActionRequests(float dt)
{
    // receive "last minute signals"
    DynamicCharacterController::HitInfo *hitInfo = char_contr->getHitInfo();
    if (hitInfo->isHit)
    {
        signal_stack.push_back(sSignal::sGotHit);
        hitInfo->isHit = false;
        //std::cout << "received hit signal\n";
    }

    // Check if on ground
    bool on_ground = char_contr->onGround();

    sSignal temp_prev_signal = doing.signal;

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
//        if (top_pri >= doing.signal )
//        {


            switch (top_pri)
            {
                case sSignal::sMove: // most likely
                {
                    if (on_ground && !state.moveInterrupted)
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

                        float forw_displ = 0.0;
                        float left_displ = 0.0;

                        float forw_speed = 0.0;
                        float left_speed = 0.0;

                        // if shift -> run
                        if (state.dirflags==1 || state.dirflags==13)    // forward
                        {
                            if (state.isShiftDown)
                            {
                                playAnim(Anim::RunForward);
                                forw_speed = runspeed;
                            }
                            else
                            {
                                if (state.inCombat)
                                    playAnim(Anim::WalkForward1H);      // This animation is crap...
                                else
                                    playAnim(Anim::WalkForward);
                                forw_speed = walkspeed;
                            }
                        }
                        if (state.dirflags==4 || state.dirflags==7)     // left
                        {
                            if (state.isShiftDown && !state.inCombat)
                            {
                                playAnim(Anim::RunLeft);
                                left_speed = runspeed;
                            }
                            else
                            {
                                if (state.inCombat)
                                    playAnim(Anim::StrafeLeft1H);
                                else
                                    playAnim(Anim::WalkLeft);
                                //playAnim(Anim::WalkLeft);
                                left_speed = walkspeed;
                            }
                        }
                        if (state.dirflags==8 || state.dirflags==11)    // right
                        {
                            if (state.isShiftDown && !state.inCombat)
                            {
                                playAnim(Anim::RunRight);
                                left_speed = -runspeed;
                            }
                            else
                            {
                                if (state.inCombat)
                                    playAnim(Anim::StrafeRight1H);
                                else
                                    playAnim(Anim::WalkRight);
                                left_speed = -walkspeed;
                            }
                        }
                        if (state.dirflags==5)                          // left diag
                        {
                            if (state.isShiftDown)
                            {
                                playAnim(Anim::RunLeftDiag);
                                left_speed = 0.70710678*runspeed;
                                forw_speed = 0.70710678*runspeed;
                            }
                            else
                            {
                                playAnim(Anim::WalkLeftDiag);
                                left_speed = 0.70710678*walkspeed;
                                forw_speed = 0.70710678*walkspeed;
                            }
                        }
                        if (state.dirflags==9)                        // right diag
                        {
                            if (state.isShiftDown)
                            {
                                playAnim(Anim::RunRightDiag);
                                left_speed = -0.70710678*runspeed;
                                forw_speed = 0.70710678*runspeed;
                            }
                            else
                            {
                                playAnim(Anim::WalkRightDiag);
                                left_speed = -0.70710678*walkspeed;
                                forw_speed = 0.70710678*walkspeed;
                            }
                        }
                        if (state.dirflags==2 || state.dirflags==14)                          // backwards
                        {
                            // cannot run backwards
                            playAnim(Anim::WalkBackward);
                            forw_speed = -walkspeed;
                        }
                        if (state.dirflags==6)                          // back left
                        {
                            // cannot run backwards
                            playAnim(Anim::WalkBackLeft);
                            forw_speed = -0.70710678*walkspeed;
                            left_speed = 0.70710678*walkspeed;
                        }
                        if (state.dirflags==10)                          // back right
                        {
                            // cannot run backwards
                            playAnim(Anim::WalkBackRight);
                            forw_speed = -0.70710678*walkspeed;
                            left_speed = -0.70710678*walkspeed;
                        }
                        //Object3d::moveForward(forw_displ, 0.f);
                        //Object3d::moveLeft(left_displ, 0.f);

                        // Physics
                        //glm::vec3 displacement = forw_displ*glm::normalize(getDir()) + left_displ*glm::normalize(getLeft());

                        glm::vec3 velocity_setpoint = forw_speed*glm::normalize(getDir()) + left_speed*glm::normalize(getLeft());

                        char_contr->velocitySetpoint(velocity_setpoint);

                    } // if (on_ground)

                } break; // case
                case sSignal::sAttack:
                {
                    float speed = 1.0;

                    snd_emitter->emitSound("swosh3.flac");

                    if (doing.signal == sMove) state.moveInterrupted = true;

                    if (state.left_sweep<0.0)
                    {
                        doing = {sAttack, getAnimDuration(Anim::SwingLeft1H)/speed, false};
                        playAnim(Anim::SwingLeft1H, true, speed);
                    }
                    else
                    {
                        doing = {sAttack, getAnimDuration(Anim::SwingRight1H)/speed, false};
                        playAnim(Anim::SwingRight1H, true, speed);
                    }
                } break;
                case sSignal::sDodge:
                {
                    float speed = 1.0;

                    snd_emitter->emitSound("dodge1.flac");

                    if (doing.signal == sMove) state.moveInterrupted = true;

                    doing = {sDodge, getAnimDuration(Anim::DodgeBack)/speed, false};

                    playAnim(Anim::DodgeBack, true, speed);
                } break;
                case sSignal::sGotHit:
                {
                    float speed = 1.0;

 //                   snd_emitter->emitSound("dodge1.flac");

                    if (doing.signal == sMove) state.moveInterrupted = true;

                    doing = {sGotHit, getAnimDuration(Anim::DodgeBack)/speed, false};

                    playAnim(Anim::DodgeBack, true, speed);
                } break;
                case sSignal::sBlock:
                {
//                    if (doing.signal != sBlock)
//                    {
                        float speed = 1.0;
                        snd_emitter->emitSound("swosh2.flac");

                        if (doing.signal == sMove) state.moveInterrupted = true;

                        if (state.left_sweep<=0.0)
                        {
                            doing = {sBlock, getAnimDuration(Anim::ParryRight1H)/speed, false};
                            playAnim(Anim::ParryRight1H, true, speed);
                        }
                        else
                        {
                            doing = {sBlock, getAnimDuration(Anim::ParryLeft1H)/speed, false};
                            playAnim(Anim::ParryLeft1H, true, speed);
                        }
//                    }

                } break;
                case sSignal::sJump:
                {
                    float speed = 1.0;

                    snd_emitter->emitSound("jump1.flac");

                    if (doing.signal == sMove) state.moveInterrupted = true;

                    //       signal         time left                    main result triggered
                    doing = {sJump, getAnimDuration(Anim::JumpUp)/speed, false};

                    playAnim(Anim::JumpUp, true, speed);

                    //char_contr->jump();
                } break;
            } // switch
        //} // if top_sig > doing.doing

    }
    else // No signal received
    {
        if (doing.signal == sMove)
            doing.signal = sNothing;
    }


    // Start blending into idle when only "blend time" remains
    // if we set 0.0 here, blend-to-idle will start at 0.0, and
    // the old animation will start replaying (for "blend time"
    // more seconds, this does not look good, so we start blending
    // into idle while there is still some time remaining on the old
    // animation
    if (doing.time < Actor::blend_time && doing.signal != sMove) // finished whatever was doing
    {
         // pre-emptively blend into idle
        if (state.inCombat)
            playAnim(Anim::Idle1H);
        else
            playAnim(Anim::Idle);


        if (doing.time < 0.0) // but the ACTION is not finished until time 0.0
        {
            // Then do nothing
            doing = {sNothing, 0.000};
        }
    }
    // else keep on playing whatever animation was playing

    //if (doing.signal != sMove && doing.signal != sJump)
    if (doing.signal != sMove)
    {
        char_contr->velocitySetpoint(glm::vec3(0.0, 0.0, 0.0));
    }

    //if (state.dirflags == dirflag::none)
    if (state.dirflags>state.prev_dirflags || state.dirflags == dirflag::none)
    {
        state.moveInterrupted = false;
    }

    state.prev_dirflags = state.dirflags;

    // follow up on the currently doing action (could implement some generic action interface later)
    switch (doing.signal)
    {
        case sSignal::sMove:
        {
            // do nothing
        } break;
        case sSignal::sAttack:
        {
            if (doing.time < Actor::blend_time) state.moveInterrupted = false;

            // delayed lunge
            if (!doing.triggered && doing.time < 0.75)
            {
                char_contr->lunge(getDir());
                doing.triggered = true;
                char_contr->testThreatRegion();
            }

        } break;
        case sSignal::sDodge:
        {
            if (doing.time < Actor::blend_time) state.moveInterrupted = false;

            // delayed lunge back
            if (!doing.triggered && doing.time < 0.6)
            {
                char_contr->lunge(-getDir());
                doing.triggered = true;
            }
        } break;
        case sSignal::sBlock:
        {
            if (doing.time < Actor::blend_time) state.moveInterrupted = false;

            // delayed lunge back
            if (!doing.triggered && doing.time < 0.75)
            {
                char_contr->lunge(-getDir());
                doing.triggered = true;
            }
        } break;
        case sSignal::sJump:
        {
            if (doing.time < Actor::blend_time) state.moveInterrupted = false;

            // delayed jump
            if (!doing.triggered && doing.time < 0.8)
            {
                char_contr->jump(getDir());
                doing.triggered = true;
            }

        } break;
    }

    char_contr->applyMoveController();
                                           // in front     up
    char_contr->updateThreatRegionTransf(pos+1.25f*getDir()+glm::vec3(0.0, 1.0, 0.0), rot);

    // Get ready for next frame

    // clear signal stack
    signal_stack.clear();

    // shift is not held down generally
    state.isShiftDown = false;

    // clear bitflags
    state.dirflags = dirflag::none;

    // store the previous signal
//    state.prev_signal = temp_prev_signal;
}

DynamicCharacterController* Creature::getCharContr()
{
    return char_contr;
}

void Creature::notifyGotHit()
{
    std::cout << "gotHit\n";
}

void Creature::updateTransformation()
{
    pos = char_contr->getWorldPos();
}

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
    state.up_sweep = degrees;
    // Creatures can not be rotated up
    look_rot = glm::rotate(look_rot, 3.14159265f*degrees/180.f, glm::vec3(1.0, 0.0, 0.0));
}

void Creature::rotateLeft(float degrees, float dt_unused)
{
    // Object3d::rotateLeft(check_sign, dt_unused);
    state.left_sweep = degrees;


    glm::vec3 axis = glm::inverse(glm::mat3_cast(look_rot)) * glm::vec3(0.0, 1.0, 0.0);
    look_rot = glm::rotate(look_rot, 3.14159265f*degrees/180.f, axis);

    // make this a controller
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

void Creature::stance()
{
    state.inCombat = !state.inCombat;
    std::cout << "changing stance \n";
}
