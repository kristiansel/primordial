#include "object3d.h"

Object3d::Object3d()
{
    pos = vec3(0.0, 0.0, 0.0);
//    dir = vec3(0.0, 0.0, -1.0);
//    up = vec3(0.0, 1.0, 0.0);
    rot = glm::quat(1.0, 0.0, 0.0, 0.0);
    scale = vec3(1.0, 1.0, 1.0);
}

Object3d::~Object3d()
{
    //dtor
}

void Object3d::moveForward(float speed)
{
//    cout<<"calling forward!\n";
//    Transform::forward(speed, pos, getDir(), vec3(0.0, 1.0, 0.0));

    pos +=(speed)*getDir();
}

void Object3d::strafeLeft(float speed)
{
//    cout<<"calling strafeLeft!\n";
//    Transform::strafe_left(speed, pos, dir, up);

//    vec3 left = -glm::normalize(glm::cross(dir,up));
    pos -=(speed)*glm::normalize(glm::cross(getDir(), vec3(0.0, 1.0, 0.0)) );
}

void Object3d::panUp(float degrees)
{
//    cout<<"calling panUp!\n";

//    vec3 axis;
//    axis=glm::normalize(glm::cross(getDir(), vec3(0.0, 1.0, 0.0) ));


    rot = glm::rotate(rot, 3.14159265f*degrees/180.f, vec3(1.0, 0.0, 0.0));


//    Transform::up(degrees, dir, up);
}

void Object3d::panLeft(float degrees)
{

    vec3 axis = glm::inverse(glm::mat3_cast(rot)) * vec3(0.0, 1.0, 0.0);

    rot = glm::rotate(rot, 3.14159265f*degrees/180.f, axis);

}

vec3 Object3d::getDir()
{
    return glm::mat3_cast(rot) * vec3(0.0, 0.0, -1.0);
}


void Object3d::setDir()
{
  /// Fill out if needed
}
