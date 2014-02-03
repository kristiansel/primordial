#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

mat4 Camera::getModelViewMatrix()
{
    mat4 mv = Transform::lookAt(pos, dir, up) ;
    mv = glm::transpose(mv) ;
    return mv;
}
