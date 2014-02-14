#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

glm::mat4 Camera::getModelViewMatrix()
{
    /// Might be possible to optimize this
    glm::mat4 tr2 = glm::translate(glm::mat4(1.0), pos);
    glm::mat4 rt2 = glm::mat4_cast(rot);
    glm::mat4 mv = glm::inverse(tr2 * rt2);
//    mm::matPrint( mv2 );
    return mv;
}
