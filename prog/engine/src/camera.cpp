#include "camera.h"

Camera::Camera()
{
//    fovy = 40; // field of vertical view
//    aspect = 1; // Ahhh! Where is this used?
//    nearz = 0.1; // near z clipping plane
//    farz = 200; // far z clipping plane
}

Camera::~Camera()
{

}

//Camera::Camera(float fovy, float aspect, float nearz, float farz)
//{
//    setPerspective(fovy, aspect, nearz, farz);
//}
//
//void Camera::setPerspective(float fovy_in, float aspect_in, float nearz_in, float farz_in)
//{
//    fovy = fovy_in;
//    aspect = aspect_in;
//    nearz = nearz_in;
//    farz = farz_in;
//}
//
mat4 Camera::getModelViewMatrix()
{
    mat4 mv = Transform::lookAt(pos, dir, up) ;
    mv = glm::transpose(mv) ;
    return mv;
}
