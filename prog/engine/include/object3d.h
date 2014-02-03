#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <glm/glm.hpp>
#include "transform.h"

class Object3d
{
public:
    Object3d();
    virtual ~Object3d();

    vec3 pos;
    vec3 dir;
    vec3 up;

    vec3 scale;


    void moveForward(float speed);
    void strafeLeft(float speed);
    void panUp(float degrees);
    void panLeft(float degrees);

    /// DONT USE THESE
//    void rotateLeft(float degrees);
//    void rotateLeft(float degrees, vec3 point);
//    void rotateUp(float degrees, vec3 point);

protected:
private:
};

#endif // OBJECT3D_H
