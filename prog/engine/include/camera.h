#ifndef CAMERA_H
#define CAMERA_H

#include "object3d.h"

class Camera: public Object3d
{
public:
    Camera();
    virtual ~Camera();

//    glm::mat4 getTransformMatrix() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getViewProjectionMatrix() const;


    float fovy;
    float aspect;
    float nearz;
    float farz;
};

#endif // CAMERA_H
