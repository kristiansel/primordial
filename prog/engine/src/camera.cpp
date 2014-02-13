#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

mat4 Camera::getModelViewMatrix()
{
    mat4 tr2 = glm::translate(mat4(1.0), pos);
//    mat4 rt2 = glm::transpose(Transform::objectRot(vec3(0.0, 1.0, 0.0), dir));
    mat4 rt2 = glm::mat4_cast(rot);
    mat4 mv = glm::inverse(tr2 * rt2);
//    mm::matPrint( mv2 );
    return mv;
}
