#ifndef CAMERA_H
#define CAMERA_H

#include "object3d.h"

class Camera: public Object3d
{
public:
    Camera();
    Camera(float fovy_in, float aspect_in, float nearz_in, float farz_in);
    virtual ~Camera();

    void loadDefault();
    void setPerspective(float fovy_in, float aspect_in, float nearz_in, float farz_in);
    void setSpeed(float speed);

    mat4 getModelViewMatrix();

    float fovy;
    float aspect;
    float nearz;
    float farz;

protected:
private:
    // perspective properties

};

#endif // CAMERA_H
