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

//void Object3d::moveForward(float speed)
//{
//    pos +=(speed)*getDir();
//}

void Object3d::strafeLeft(float speed)
{
    pos -=(speed)*glm::normalize(glm::cross(getDir(), glm::vec3(0.0, 1.0, 0.0)) );
}

void Object3d::panUp(float degrees)
{
    /// There might be a more efficient way of doing this if this
    /// ever becomes a bottle-neck (suggest: quaternion mult)
    rot = glm::rotate(rot, 3.14159265f*degrees/180.f, glm::vec3(1.0, 0.0, 0.0));
}

void Object3d::panLeft(float degrees)
{
    /// There might be a more efficient way of doing this if this
    /// ever becomes a bottle-neck (suggest: quaternion mult)
    glm::vec3 axis = glm::inverse(glm::mat3_cast(rot)) * glm::vec3(0.0, 1.0, 0.0);
    rot = glm::rotate(rot, 3.14159265f*degrees/180.f, axis);

}

glm::vec3 Object3d::getDir()
{
    return glm::mat3_cast(rot) * glm::vec3(0.0, 0.0, -1.0);
}


void Object3d::setDir(glm::vec3 u) /// Trust that u is normalized
{
    /// no rotation vector
    glm::vec3 v = glm::vec3(0.0, 0.0, -1.0);

    /// study this! (might have to reverse cross order)
    glm::vec3 w = glm::cross(u, v);
    glm::quat q = glm::quat(1.f + glm::dot(u, v), w.x, w.y, w.z);

    /// Set rotation
    rot = glm::normalize(q);
}

void Object3d::moveForward(float speed)
{
    pos +=(speed)*getDir();
}

void Object3d::moveLeft(float speed)
{
    pos -=(speed)*glm::normalize(glm::cross(getDir(), glm::vec3(0.0, 1.0, 0.0)) );
}

void Object3d::rotateUp(float degrees)
{
    /// There might be a more efficient way of doing this if this
    /// ever becomes a bottle-neck (suggest: quaternion mult)
    rot = glm::rotate(rot, 3.14159265f*degrees/180.f, glm::vec3(1.0, 0.0, 0.0));
}

void Object3d::rotateLeft(float degrees)
{
    /// There might be a more efficient way of doing this if this
    /// ever becomes a bottle-neck (suggest: quaternion mult)
    glm::vec3 axis = glm::inverse(glm::mat3_cast(rot)) * glm::vec3(0.0, 1.0, 0.0);
    rot = glm::rotate(rot, 3.14159265f*degrees/180.f, axis);
}
