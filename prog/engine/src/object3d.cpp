#include "object3d.h"

Object3d::Object3d()
{
    pos = glm::vec3(0.0, 0.0, 0.0);
    rot = glm::quat(1.0, 0.0, 0.0, 0.0);
    scale = glm::vec3(1.0, 1.0, 1.0);
}

Object3d::~Object3d()
{
    //dtor
}

glm::mat4 Object3d::getTransformMatrix() const
{
    glm::mat4 pos_mat = glm::translate(glm::mat4(1.0), pos);
    glm::mat4 rot_mat = glm::mat4_cast(rot);
    glm::mat4 sca_mat = glm::scale(glm::mat4(1.0), scale);

    return pos_mat * rot_mat * sca_mat;
}

glm::vec3 Object3d::getDir() const
{
    return glm::mat3_cast(rot) * glm::vec3(0.0, 0.0, -1.0);
}


void Object3d::setDir(glm::vec3 u) /// Trust that u is normalized
{
    /// no rotation vector
    glm::vec3 v = glm::vec3(0.0, 0.0, -1.0);

    /// study this! (might have to reverse cross order)
    glm::vec3 w = glm::cross(v, u);
    glm::quat q = glm::quat(1.f + glm::dot(u, v), w.x, w.y, w.z);

    /// Set rotation
    rot = glm::normalize(q);
}

void Object3d::moveForward(float meters, float dt_unused)
{
    pos +=(meters)*getDir();
}

void Object3d::moveLeft(float meters, float dt_unused)
{
    pos -=(meters)*glm::normalize(glm::cross(getDir(), glm::vec3(0.0, 1.0, 0.0)) );
}

void Object3d::rotateUp(float degrees, float dt_unused)
{
    /// There might be a more efficient way of doing this if this
    /// ever becomes a bottle-neck (suggest: quaternion mult)
    rot = glm::rotate(rot, 3.14159265f*degrees/180.f, glm::vec3(1.0, 0.0, 0.0));
}

void Object3d::rotateLeft(float degrees, float dt_unused)
{
    /// There might be a more efficient way of doing this if this
    /// ever becomes a bottle-neck (suggest: quaternion mult)
    glm::vec3 axis = glm::inverse(glm::mat3_cast(rot)) * glm::vec3(0.0, 1.0, 0.0);
    rot = glm::rotate(rot, 3.14159265f*degrees/180.f, axis);
}

//void Object3d::faceInDir(const glm::vec3 &dir_in)
//{
//    // Very naive implementation...
//    glm::vec3 no_rot_vec = glm::vec3(0.0, 0.0, -1.0);
//    rot = glm::quat(1.0, 0.0, 0.0, 0.0);
//
//    // first rotate around y
//    glm::vec3 dir_in_proj_xz = glm::normalize(glm::vec3(dir_in.x, 0.0, dir_in.z));
//    float angle_rad_y = acosf(glm::dot(dir_in_proj_xz, no_rot_vec));
//    rot = glm::rotate(rot, angle_rad_y, glm::vec3(0.0, 1.0, 0.0));
//
//    // then rotate around x
//    glm::vec3 dir_in_proj_yz = glm::normalize(glm::vec3(0.0, dir_in.y, dir_in.z));
//    float angle_rad_x = acosf(glm::dot(dir_in_proj_yz, no_rot_vec));
//    rot = glm::rotate(rot, angle_rad_x, glm::vec3(1.0, 0.0, 0.0));
//}

