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

class Object3d : virtual public SignalReceiver
{
public:
    Object3d();
    virtual ~Object3d();

    glm::vec3 getDir() const;
    void setDir(glm::vec3 u);

    glm::mat4 getTransformMatrix() const;

    // Object3d implementation of MoveSignalReceiver
    void moveForward(float meters, float dt_unused);
    void moveLeft(float meters, float dt_unused);
    void rotateUp(float degrees, float dt_unused);
    void rotateLeft(float degrees, float dt_unused);

    void faceInDir(const glm::vec3 &dir_in);

public:
    glm::vec3 pos;
    glm::quat rot;
    glm::vec3 scale;

protected:
private:
};

#endif // OBJECT3D_H
