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

void Object3d::moveForward(float speed)
{
    pos +=(speed)*getDir();
}

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


void Object3d::setDir()
{
  /// Fill out if needed
}
