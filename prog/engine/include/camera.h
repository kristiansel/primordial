#ifndef CAMERA_H
#define CAMERA_H

#include "object3d.h"
//#include "signals.h"

#include "quadtree.hpp"

class Camera: public Object3d //, public MoveSignalReceiver
{
public:
    Camera();
    virtual ~Camera();

//    glm::mat4 getTransformMatrix() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getViewProjectionMatrix() const;

    QuadFrustum get2dViewFrustum() const;
//
//    // Camera implementation of MoveSignalReceiver
//    void moveForward(float amount);
//    void moveLeft(float amount);
//    void rotateUp(float amount);
//    void rotateLeft(float amount);

    float fovy;
    float aspect;
    float nearz;
    float farz;


};

#endif // CAMERA_H
