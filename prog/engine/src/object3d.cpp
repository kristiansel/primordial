#include "object3d.h"

Object3d::Object3d()
{
    pos = vec3(0.0, 0.0, 0.0);
    dir = vec3(0.0, 0.0, -1.0);
    up = vec3(0.0, 1.0, 0.0);
    scale = vec3(1.0, 1.0, 1.0);
}

Object3d::~Object3d()
{
    //dtor
}

void Object3d::moveForward(float speed)
{
//    cout<<"calling forward!\n";
    Transform::forward(speed, pos, dir, up);
}

void Object3d::strafeLeft(float speed)
{
//    cout<<"calling strafeLeft!\n";
    Transform::strafe_left(speed, pos, dir, up);
}

void Object3d::panUp(float degrees)
{
//    cout<<"calling panUp!\n";
    Transform::up(degrees, dir, up);
}

void Object3d::panLeft(float degrees)
{
//    cout<<"calling panLeft!\n";
    Transform::left(degrees, dir, up);
}


//void Object3d::rotateLeft(float degrees) {
////    Transform::left(degrees, pos, up);
//    mat3 rot = Transform::rotate(degrees, up);
//    dir = rot*dir;
//}
//
//void Object3d::rotateLeft(float degrees, vec3 point)
//{
//    Transform::left_cb(degrees, pos, dir, up, point);
//    up = vec3(0, 1, 0);
//}
//
//void Object3d::rotateUp(float degrees, vec3 point)
//{
//
//    Transform::up_cb(degrees, pos, dir, up, point);
//    up = vec3(0, 1, 0);
//
//
//}
