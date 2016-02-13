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
    doing( {sSignal::sNothing,
            0.000,
            false}),
    candidate( {sSignal::sNothing}),
    char_contr(new DynamicCharacterController),
    snd_emitter(new SoundEmitter),
    target_creature(nullptr),
    m_aiAgent(nullptr),
    m_aiWorld(nullptr)
{
    //ctor
//    signal_stack.reserve(signal_stack_capacity);

    // initialize signal stack memory
    for (unsigned int i = 0; i<SIG_REG_SIZE; i++)
    {
        doing.reg[i] = sSignal::sNothing;
        candidate.reg[i] = sSignal::sNothing;
    }
    // can not set user of the character controller here... until the char controller is initialized
}

Creature::~Creature()
{
    //dtor
    char_contr->destroy();
    delete char_contr;
    delete snd_emitter;

    if (m_aiAgent && m_aiWorld)
    {
        m_aiWorld->removeAgent(m_aiAgent);
        delete m_aiAgent;
    }
}

void Creature::resolveActionRequests(float dt)
{
    // Check if on ground
    bool on_ground = char_contr->onGround();

    // sSignal temp_prev_signal = doing.signal;

    // decrement the time
    doing.time -=dt;
    if (candidate.signal != sSignal::sClearSignal)
//    if (!signal_stack.empty())
    {
//        std::sort(signal_stack.begin(), signal_stack.end());
//
//        // get reference to top prioritized signal
//        sSignal &top_pri = signal_stack.back();
//
//        // Gameplay decision: If singals should be hierarchical (uncomment)
//        // or the new signal should always interupt current (comment)
//        // if (top_pri > doing.signal)


        switch (candidate.signal)
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

                    //float forw_displ = 0.0;
                    //float left_displ = 0.0;

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

                if ((doing.signal!=sSignal::sAttack) || (doing.time < 0.68)) // doing.time < triggertime
                //if(1)
                {
                    snd_emitter->emitSound("swosh3.flac");

                    state.moveInterrupted = true;

                    float eps = 0.000001;

                    if (state.left_sweep<-eps)
                    {
                        doing = {sAttack, getAnimDuration(Anim::SwingLeft1H)/speed, false};
                        playAnim(Anim::SwingLeft1H, true, speed);
                        doing.reg[0] = -90.0; // store angle
                    }
                    else if (state.left_sweep>eps)
                    {
                        doing = {sAttack, getAnimDuration(Anim::SwingRight1H)/speed, false};
                        playAnim(Anim::SwingRight1H, true, speed);
                        doing.reg[0] = 90.0; // store angle
                    }
                    else // state.left_sweep ~= 0.0
                    {
                        int dice = rand() % 100;
                        if (dice >= 50)
                        {
                            doing = {sAttack, getAnimDuration(Anim::SwingLeft1H)/speed, false};
                            playAnim(Anim::SwingLeft1H, true, speed);
                            doing.reg[0] = -90.0; // store angle
                        }
                        else
                        {
                            doing = {sAttack, getAnimDuration(Anim::SwingRight1H)/speed, false};
                            playAnim(Anim::SwingRight1H, true, speed);
                            doing.reg[0] = 90.0; // store angle
                        }
                    }
                }
                else
                {
                    std::cout << "Attack cancelled because already attacking\n";
                }

            } break;
            case sSignal::sDodge:
            {
                float speed = 1.0;

                snd_emitter->emitSound("dodge1.flac");

                state.moveInterrupted = true;

                doing = {sDodge, getAnimDuration(Anim::DodgeBack)/speed, false};

                playAnim(Anim::DodgeBack, true, speed);
            } break;
            case sSignal::sGotHit:
            {
                float speed = 1.0;

                snd_emitter->emitSound("fleshrip.aiff");

                // WTB a consistent and robust signalling system...
                m_aiAgent->interrupt();

                stats.health -=10.0;

                state.moveInterrupted = true;

                doing = {sGotHit, getAnimDuration(Anim::DodgeBack)/speed, false};

                playAnim(Anim::DodgeBack, true, speed);

                // Copy the candidate register over into the doing.reg
                doing.reg = candidate.reg;
//                doing.reg[0] = candidate.reg[0];
//                doing.reg[1] = candidate.reg[1];
//                doing.reg[2] = candidate.reg[2];

//                glm::vec3 hit_dir = glm::vec3(doing.reg[0], doing.reg[1], doing.reg[2]);
            } break;
            case sSignal::sParried:
            {
                snd_emitter->emitSound("clang.aiff");

//                    doing.signal = sBlock;
                doing.signal = sParried;
                doing.time = 1.0;

            } break;
            case sSignal::sBlock:
            {
//                    if (doing.signal != sBlock)
//                    {
                    float speed = 1.0;
                    snd_emitter->emitSound("swosh2.flac");

                    state.moveInterrupted = true;

                    // Copy the candidate register over into the doing.reg
                    doing.reg = candidate.reg;
                    float from_angle = doing.reg[0];

                    if (from_angle<0.0)
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

                state.moveInterrupted = true;

                //       signal         time left                    main result triggered
                doing = {sJump, getAnimDuration(Anim::JumpUp)/speed, false};

                playAnim(Anim::JumpUp, true, speed);

                //char_contr->jump();
            } break;
            default:
            {
                // do nothing
            }
        } // switch
    } // if signal received
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

    if (doing.signal == sNothing)
    //if (doing.signal != sMove )
    {
        char_contr->velocitySetpoint(glm::vec3(0.0, 0.0, 0.0));
    }

    // alow move to interrupt... other things (def better with this off)
//    if (state.dirflags>state.prev_dirflags || state.dirflags == dirflag::none)
//    {
//        state.moveInterrupted = false;
//    }

    state.prev_dirflags = state.dirflags;

    // follow up on the currently doing action (could implement some generic
    // action interface later)
    switch (doing.signal)
    {
        case sSignal::sMove:
        {
            // do nothing
        } break;
        case sSignal::sAttack:
        {
            float trigger_time = 0.68; // very short reaction time?

            if (doing.time < Actor::blend_time) state.moveInterrupted = false;

            if (doing.time > trigger_time)
            { // before triggered
                char_contr->velocitySetpoint(glm::vec3(0.0, 0.0, 0.0));
            }
            else
            {
                char_contr->velocitySetpoint(getDir()*doing.time/trigger_time*3.f);
            }

            if (!doing.triggered && doing.time < trigger_time)
            {
                //char_contr->lunge(getDir());
                doing.triggered = true;
                //char_contr->testThreatRegion();

                char_contr->forAllThreatenedDo(
                                                   [&] (void* in)
                                                   {
                                                       Creature* target = (Creature*)in;
                                                       glm::vec3 hit_dir = glm::normalize(target->pos - this->pos);

                                                       float angle = doing.reg[0]; // unpack the stored angle

                                                       //          hit_by, angle, direction
                                                       target->hit( {this, angle, hit_dir} );
                                                   }
                                               );
            }

        } break;
        case sSignal::sDodge:
        {
            float trigger_time = 0.98;

            if (doing.time < Actor::blend_time) state.moveInterrupted = false;

            if (doing.time > trigger_time)
            { // before triggered
                char_contr->velocitySetpoint(glm::vec3(0.0, 0.0, 0.0));
            }
            else
            {
                char_contr->velocitySetpoint(-getDir()*1.3f*pow(doing.time/trigger_time*2.f, 2.f));
            }

            // delayed lunge back
            if (!doing.triggered && doing.time < trigger_time)
            {
                //char_contr->lunge(-getDir());
                doing.triggered = true;
            }
        } break;
        case sSignal::sGotHit:
        {
            // No triggers

            if (doing.time < Actor::blend_time) state.moveInterrupted = false;

            // movement in direction of hit
//            std::cout << "moving when getting hit\n";
            glm::vec3 hit_dir = glm::vec3(doing.reg[0], doing.reg[1], doing.reg[2]);
            char_contr->velocitySetpoint(hit_dir*0.1f*pow(doing.time*2.f, 4.f));

        } break;
        case sSignal::sBlock:
        {
//            float trigger_time = 0.98;

            if (doing.time < Actor::blend_time) state.moveInterrupted = false;

//            if (doing.time > trigger_time)
//            { // before triggered
                char_contr->velocitySetpoint(glm::vec3(0.0, 0.0, 0.0));
//            }
//            else
//            {
////                char_contr->velocitySetpoint(-getDir()*doing.time/trigger_time*3.f);
//            }

            // delayed lunge back
//            if (!doing.triggered && doing.time < trigger_time)
//            {
//                char_contr->lunge(-getDir());
//                doing.triggered = true;
//            }
        } break;
        case sSignal::sParried:
        {
//            float trigger_time = 0.98;

            if (doing.time < Actor::blend_time) state.moveInterrupted = false;

//            if (doing.time > trigger_time)
//            { // before triggered
//                char_contr->velocitySetpoint(glm::vec3(0.0, 0.0, 0.0));
//            }
//            else
//            {
                char_contr->velocitySetpoint(-getDir()*0.7f*pow(doing.time*2.f, 2.f));
//            }

        } break;
        case sSignal::sJump:
        {
            if (doing.time < Actor::blend_time) state.moveInterrupted = false;

            // delayed jump
            if (!doing.triggered && doing.time < 0.8)
            {
                char_contr->jump(getDir());
                snd_emitter->emitSound("jump1.flac");
                doing.triggered = true;
            }

        } break;
        default:
        {
            // do nothing
        }
    } // switch

    char_contr->applyMoveController();
                                           // in front     up
    char_contr->updateThreatRegionTransf(pos+1.25f*getDir()+glm::vec3(0.0, 1.0, 0.0), rot);

    // Get ready for next frame

    // clear signal stack
//    signal_stack.clear();
    candidate.signal = sSignal::sClearSignal;

    // shift is not held down generally
    state.isShiftDown = false;

    // clear bitflags
    state.dirflags = dirflag::none;

    // have to do this somewhere:
    // update ai where we are and what we are looking at
    m_aiAgent->setPos(pos);
    m_aiAgent->setDir(getLookDir()); // getDir or getLookDir?
}

DynamicCharacterController* Creature::getCharContr()
{
    return char_contr;
}

float Creature::getHealth() const
{
    return stats.health;
}

void Creature::setHealth(float health)
{
    stats.health = health;
}

void Creature::updateTransformation()
{
    pos = char_contr->getWorldPos();
}

void Creature::connectAI(ai::Agent* aiAgent, ai::World* aiWorld)
{
    m_aiAgent = aiAgent;
    m_aiWorld = aiWorld;

    m_aiAgent->setUserPointer(this);
}

bool Creature::isAttacking()
{
    // if still attacking and triggered is true, then the threat is over
    return (doing.signal==sAttack && doing.triggered==false);
}

bool Creature::isBlocking()
{
    return (doing.signal==sBlock || doing.signal==sParried);
}

bool Creature::isDodging()
{
    return (doing.signal==sDodge);
}


bool Creature::isInCombat()
{
    return state.inCombat;
}

void Creature::signal_push(sSignal signal)
{
//    signal_stack.push_back(signal);
    if (signal > candidate.signal) // if the signal has higher priority
    {
        candidate.signal = signal;
    }
}

void Creature::signal_push(sSignal signal, std::array<float, SIG_REG_SIZE> args)
{
    signal_push(signal); // just one place to modify later if changes are required

    // store the arguments
    candidate.reg = args;
//    candidate.reg[0] = args[0];
//    candidate.reg[1] = args[1];
//    candidate.reg[2] = args[2];
}

void Creature::hit(HitInfo hit_info)
{
    if (doing.signal==sBlock)
    {
        signal_push(sSignal::sParried);
        hit_info.hitBy->hitWasBlocked();
    }
    else if (doing.signal==sDodge)
    {
        // keep dodging/register no hit
    }
    else
    { // Actually got hit
        SignArgType to_reg = {hit_info.dir.x, hit_info.dir.y, hit_info.dir.z};
        signal_push(sSignal::sGotHit, to_reg);
    }
}

void Creature::hitWasBlocked()
{
//    std::cout << "hit was blocked with a margin of " << doing.time-0.68 << " secs\n";
    // This will start blending into idle
    doing.time = Actor::blend_time*1.4; // instead of stopping the animation prematurely...
    // should pause the animation for a bit
//    staggerAnim(0.1); // pause animation for 0.1 secs


    // interrupt the AI
    m_aiAgent->interrupt();
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

    signal_push(sSignal::sMove);
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
    signal_push(sSignal::sMove);
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


    // This signal should be called "swing or something like it"
    // choose attack or parry based on if threatened

    bool threatened = false;
    float from_angle = 0.0;
    char_contr->forAllThreatenedDo(
                                       [&] (void* in)
                                       {
                                           Creature* creature = (Creature*)(in);
                                           if (creature->isAttacking()) threatened = true;
                                           from_angle = -creature->doing.reg[0];
                                       }
                                   );
    if (threatened)
    {
        SignArgType args = {from_angle}; // pass the angle of the attack to the block signal
        signal_push(sSignal::sBlock, args);
    }
    else
    {
        signal_push(sSignal::sAttack);
    }
}

void Creature::dodge()
{
    signal_push(sSignal::sDodge);
}

void Creature::block()
{
    signal_push(sSignal::sBlock);
}

void Creature::shift()
{
    state.isShiftDown = true;
}

void Creature::jump()
{
    signal_push(sSignal::sJump);
}

void Creature::stance()
{
    state.inCombat = !state.inCombat;
    std::cout << "changing stance \n";
}
