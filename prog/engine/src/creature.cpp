#include "creature.h"

Creature::Creature() :
    movespeed(3.0), // m/s (1.5 walk, 3.0 jog, 6.0 run)
    //look_rot(glm::quat(1.0, 0.0, 0.0, 0.0))
    look_rot(rot),
    // isIdle, isShiftDown
    state( {false} ),
    doing( {Signal::Nothing, 0.000})
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
        Signal &top_pri = signal_stack.back();

        // Gameplay decision: If singals should be hierarchical (uncomment)
        // or the new signal should always interupt current (comment)
        // if (top_pri > doing.signal)

        // execute that signal
        switch (top_pri)
        {
        case Signal::Move: // most likely
            {
                doing = {Move, 1.0/20.0}; // keep moving for a twentieth of a second
                playAnim(Anim::RunForward);
            } break;
        case Signal::Attack:
            {
                float speed = 1.0;

                doing = {Attack, getAnimDuration(Anim::SwingRight1H)/speed};

                playAnim(Anim::SwingRight1H, true, speed);
            } break;
        case Signal::Dodge:
            {
                float speed = 1.0;

                doing = {Dodge, getAnimDuration(Anim::DodgeBack)/speed};

                playAnim(Anim::DodgeBack, true, speed);
            } break;
        case Signal::Block:
            {
                float speed = 1.0;

                doing = {Block, getAnimDuration(Anim::ParryLeft1H)/speed};

                playAnim(Anim::ParryLeft1H, true, speed);
            } break;
        case Signal::Jump:
            {
                float speed = 1.0;

                doing = {Jump, getAnimDuration(Anim::JumpUp)/speed};

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
                doing = {Nothing, 0.000};
            }
        }
        // else keep on playing whatever animation was playing
    }

    // Get ready for next frame

    // clear signal stack
    signal_stack.clear();

    // shift is not held down generally
    state.isShiftDown = false;
}

/// Animations

void Creature::moveForward(float check_sign, float dt)
{
    // Move
    float sign = (check_sign>0) ? 1.0 : -1.0 ;
    float amount = sign * movespeed * dt;

    Object3d::moveForward(amount, 0.0); // second argument unused;

//    // Blend into forward movement animation
//    if (check_sign>0)
//        playAnim(Anim::Run);
//    else
//        playAnim(Anim::Run, 0.5);
//        //playAnim(Anim::Run, -1.0); // Negative numbers for speed do not work so well...
//                                  // should probably look into timeseries for supporting
//                                  // backward animation playback

    signal_stack.push_back(Signal::Move);
//    setDir(look_dir_projected_xz);
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
//
//void Creature::setLookDir(glm::vec3 u) /// Trust that u is normalized
//{
//    /// no rotation vector
//    glm::vec3 v = glm::vec3(0.0, 0.0, -1.0);
//
//    /// study this! (might have to reverse cross order)
//    glm::vec3 w = glm::cross(v, u);
//    glm::quat q = glm::quat(1.f + glm::dot(u, v), w.x, w.y, w.z);
//
//    /// Set rotation
//    look_rot = glm::normalize(q);
//}
//
glm::quat Creature::getLookRot() const
{
    return look_rot;
}

void Creature::attack()
{
    signal_stack.push_back(Signal::Attack);
}

void Creature::dodge()
{
    signal_stack.push_back(Signal::Dodge);
}

void Creature::block()
{
    signal_stack.push_back(Signal::Block);
}

void Creature::shift()
{
    state.isShiftDown = true;
}

void Creature::jump()
{
    signal_stack.push_back(Signal::Jump);
}
