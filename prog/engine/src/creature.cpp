#include "creature.h"

Creature::Creature() :
    movespeed(3.0), // m/s (1.5 walk, 3.0 jog, 6.0 run)
    //look_rot(glm::quat(1.0, 0.0, 0.0, 0.0))
    look_rot(rot)
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
//
//void Creature::setLookRot(glm::quat q_in)
//{
//    look_rot = q_in;
//};
//
//void Creature::lookInDir(const glm::vec3 &dir_in)
//{
//    // Very naive implementation...
//    glm::vec3 no_rot_vec = glm::vec3(0.0, 0.0, -1.0);
//    look_rot = glm::quat(1.0, 0.0, 0.0, 0.0);
//
//    // first rotate around y
//    glm::vec3 dir_in_proj_xz = glm::normalize(glm::vec3(dir_in.x, 0.0, dir_in.z));
//    float angle_rad_y = acosf(glm::dot(dir_in_proj_xz, no_rot_vec));
//    look_rot = glm::rotate(look_rot, angle_rad_y, glm::vec3(0.0, 1.0, 0.0));
//
//    // then rotate around x
//    glm::vec3 dir_in_proj_yz = glm::normalize(glm::vec3(0.0, dir_in.y, dir_in.z));
//    float angle_rad_x = acosf(glm::dot(dir_in_proj_yz, no_rot_vec));
//    look_rot = glm::rotate(look_rot, angle_rad_x, glm::vec3(1.0, 0.0, 0.0));
//}
//
//void Creature::lookAt(const glm::vec3 &pos_in)
//{
//    glm::mat4
//}
