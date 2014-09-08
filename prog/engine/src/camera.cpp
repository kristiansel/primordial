#include "camera.h"

Camera::Camera() :
    fovy(40), // field of vertical view, 40
    aspect(1920.f/1080.f), // aspect ratio, 1.6
    nearz(0.6), // near z clipping plane
    farz(600) // far z clipping plane
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

QuadFrustum Camera::get2dViewFrustum(float scale, float depth_frac)  const
{
    glm::mat4 this_transf = this->getTransformMatrix();

    float half_angle_rad = 3.14159265*fovy/180.f/2.f;

    float del_xn = aspect*nearz*tan(half_angle_rad);
    float del_xf = aspect*farz*tan(half_angle_rad);

    glm::vec4 p0 = this_transf * glm::vec4(-del_xn*scale, 0, -nearz/scale, 1.0);
    glm::vec4 p1 = this_transf * glm::vec4(del_xn*scale, 0, -nearz/scale, 1.0);
    glm::vec4 p2 = this_transf * glm::vec4(del_xf*scale, 0, -farz*depth_frac, 1.0);
    glm::vec4 p3 = this_transf * glm::vec4(-del_xf*scale, 0, -farz*depth_frac, 1.0);


    return QuadFrustum({p0, p1, p2, p3});


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
