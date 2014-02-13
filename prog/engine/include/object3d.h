#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
//#include "transform.h"
// #include "quaternion.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Object3d
{
public:
    Object3d();
    virtual ~Object3d();

    vec3 pos;
//    vec3 dir;
//    vec3 up;
    glm::quat rot;
    //Quat rot;


    vec3 scale;

    vec3 getDir();
    void setDir();

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
