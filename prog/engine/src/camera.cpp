#include "camera.h"

Camera::Camera() :
    fovy(40), // field of vertical view
    aspect(1.6), // aspect ratio
    nearz(0.6), // near z clipping plane
    farz(1200) // far z clipping plane
{

}

Camera::~Camera()
{

}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::inverse(getTransformMatrix());
}

glm::mat4 Camera::getProjectionMatrix() const
{
    glm::mat4 proj_mat = glm::perspective(3.14159265f*fovy/180.f, aspect, nearz, farz);

    return proj_mat;
}

glm::mat4 Camera::getViewProjectionMatrix() const
{
    glm::mat4 view_mat = getViewMatrix();
    glm::mat4 proj_mat = getProjectionMatrix();

    return proj_mat * view_mat;
}
//
//void Camera::moveForward(float amount)
//{
//    Object3d::moveForward(amount);
//}
//
//void Camera::moveLeft(float amount)
//{
//    Object3d::moveLeft(amount);
//}
//
//void Camera::rotateUp(float amount)
//{
//    Object3d::rotateUp(amount);
//}
//
//void Camera::rotateLeft(float amount)
//{
//    Object3d::rotateLeft(amount);
//}
