#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
//#include "transform.h"
// #include "quaternion.h"

//using glm::vec3;
//using glm::vec4;
//using glm::mat4;

#include "signals.h"

class Object3d : public MoveSignalReceiver
{
public:
    Object3d();
    virtual ~Object3d();

    glm::vec3 pos;
    glm::quat rot;
    glm::vec3 scale;

    glm::vec3 getDir();
    void setDir(glm::vec3 u);

    glm::mat4 getTransformMatrix() const;

    void moveForward(float speed);
    void strafeLeft(float speed);
    void panUp(float degrees);
    void panLeft(float degrees);

//    void moveForward(float amount);
    void moveLeft(float amount);
    void rotateUp(float amount);
    void rotateLeft(float amount);

    /// DONT USE THESE
//    void rotateLeft(float degrees);
//    void rotateLeft(float degrees, glm::vec3 point);
//    void rotateUp(float degrees, glm::vec3 point);

protected:
private:
};

#endif // OBJECT3D_H
