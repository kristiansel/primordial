#include "Transform.h"
Transform::Transform()
{
};
Transform::~Transform()
{
};
void Transform::left_cb(float degrees, vec3& eye, vec3& dir, vec3& up, vec3 point)
{

    vec3 bef = glm::normalize(point - eye);
    vec3 axis;
    axis = -glm::normalize(up);
    mat3 R = rotate(degrees, axis);
    vec3 eyeM = eye - point;
    vec3 eyeRot = R*eyeM;
    eye = eyeRot + point;
    dir = glm::normalize(point - eye);
};
void Transform::up_cb(float degrees, vec3& eye, vec3& dir, vec3& up, vec3 point)
{

//    mm::vecPrint(eye);
//    mm::vecPrint(dir);
//    mm::vecPrint(up);
//    mm::vecPrint(point);

    vec3 axis;
    dir = glm::normalize(point - eye);
    axis= glm::normalize(glm::cross(dir, up));
    mat3 R = rotate(degrees, axis);
    vec3 eyeM = eye - point;
    vec3 eyeRot = R*eyeM;
    eye = eyeRot + point;
    dir = glm::normalize(point - eye);
    //up = R*up;

//    mm::vecPrint(eye);
//    mm::vecPrint(dir);
//    mm::vecPrint(up);
//    mm::vecPrint(point);
};
void Transform::left(float degrees, vec3& dir, vec3& up)
{
    vec3 axis;
    // axis = glm::normalize(up);
    axis = vec3(0.0, 1.0, 0.0);
    mat3 R = rotate(-degrees, axis);
    dir = R*dir;
    up = R*up;
};
void Transform::up(float degrees, vec3& dir, vec3& up)
{
    vec3 axis;
    axis=glm::normalize(glm::cross(dir,up));
    mat3 R = rotate(-degrees, axis);
    dir = R*dir;
    up = R*up;
};
void Transform::forward(float speed, vec3& eye, vec3& dir, vec3& up)
{
    vec3 ahead = glm::normalize(dir);
    eye +=(speed)*ahead;
};
void Transform::strafe_left(float speed, vec3& eye, vec3 &dir, vec3& up)
{
    vec3 left = -glm::normalize(glm::cross(dir,up));
    eye +=(speed)*left;
};

//mat4 Transform::quatRot(Quat local_rotation)
//{
//    Quat q = local_rotation.ret_normalized();
//    mat4 R4;
//    float w2 = q.w*q.w;
//    float xw = q.x*q.w;
//    float x2 = q.x*q.x;
//    float yw = q.y*q.w;
//    float yx = q.y*q.x;
//    float y2 = q.y*q.y;
//    float zw = q.z*q.w;
//    float zx = q.z*q.x;
//    float zy = q.z*q.y;
//    float z2 = q.z*q.z;
//
//    R4[0][0]=1-2*y2-2*z2;
//    R4[0][1]=2*yx-2*zw;
//    R4[0][2]=2*zx+2*yw;
//    R4[0][3]=0;
//    R4[1][0]=2*yx+2*zw;
//    R4[1][1]=1-2*x2-2*z2;
//    R4[1][2]=2*zy-2*xw;
//    R4[1][3]=0;
//    R4[2][0]=2*zx-2*yw;
//    R4[2][1]=2*zy+2*xw;
//    R4[2][2]=1-2*x2-2*y2;
//    R4[2][3]=0;
//    R4[3][0]=0;
//    R4[3][1]=0;
//    R4[3][2]=0;
//    R4[3][3]=1;
//    return R4; // NEEDS TESTING
//};

mat4 Transform::objectRot(vec3 up, vec3 dir)
{
    mat4 R4(1.0);
    vec3 w;
    vec3 u;
    vec3 v;
    //up = -up;
    w=-glm::normalize(dir);
    u=glm::normalize(glm::cross(up,w));
    v=glm::normalize(glm::cross(w,u));
    R4[0][0]=u.x;
    R4[0][1]=v.x;
    R4[0][2]=w.x;
    R4[0][3]=0;
    R4[1][0]=u.y;
    R4[1][1]=v.y;
    R4[1][2]=w.y;
    R4[1][3]=0;
    R4[2][0]=u.z;
    R4[2][1]=v.z;
    R4[2][2]=w.z;
    R4[2][3]=0;
    R4[3][0]=0;
    R4[3][1]=0;
    R4[3][2]=0;
    R4[3][3]=1;
    return R4;
}
mat4 Transform::lookAt(const vec3& eye, const vec3 &dir, const vec3& up)
{
    mat4 M;
    mat4 R4(1.0);
    mat4 T4(1.0);
    vec3 w;
    vec3 u;
    vec3 v;
    w=-glm::normalize(dir);
    u=glm::normalize(glm::cross(up,w));
    v=glm::normalize(glm::cross(w,u));
    M[0][0]=u.x;
    M[0][1]=u.y;
    M[0][2]=u.z;
    M[0][3]=glm::dot(-eye,u);
    M[1][0]=v.x;
    M[1][1]=v.y;
    M[1][2]=v.z;
    M[1][3]=glm::dot(-eye,v);
    M[2][0]=w.x;
    M[2][1]=w.y;
    M[2][2]=w.z;
    M[2][3]=glm::dot(-eye,w);
    M[3][0]=0;
    M[3][1]=0;
    M[3][2]=0;
    M[3][3]=1;
    return M ;
};
mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    float f=1/tan((3.14159265/180)*fovy/2);
    mat4 P(1.0);
    P[0][0]=f/aspect;
    P[0][1]=0;
    P[0][2]=0;
    P[0][3]=0;
    P[1][0]=0;
    P[1][1]=f;
    P[1][2]=0;
    P[1][3]=0;
    P[2][0]=0;
    P[2][1]=0;
    P[2][2]=(zFar+zNear)/(zNear-zFar);
    P[2][3]=(2*zFar*zNear)/(zNear-zFar);
    P[3][0]=0;
    P[3][1]=0;
    P[3][2]=-1;
    P[3][3]=0;
    return P;
};
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
    mat3 R ;
    float x=axis.x;
    float y=axis.y;
    float z=axis.z;
    float rad=3.14159265*degrees/180;
    mat3 A(cos(rad), 0, 0,
           0, cos(rad), 0,
           0, 0, cos(rad));

    mat3 B((1-cos(rad))*x*x, (1-cos(rad))*x*y, (1-cos(rad))*x*z,
           (1-cos(rad))*x*y, (1-cos(rad))*y*y, (1-cos(rad))*y*z,
           (1-cos(rad))*x*z, (1-cos(rad))*y*z, (1-cos(rad))*z*z);
    mat3 C(0, sin(rad)*(-z), sin(rad)*(y),
           sin(rad)*(z), 0, sin(rad)*(-x),
           sin(rad)*(-y), sin(rad)*(x), 0);
    R=A+B+C;
    return R;
};
//        mat4 Transform::rotate(const float degrees, const vec3& axis) {
//            mat3 R = rotate(degrees, axis);
//            mat4 P;
//            P[0][0]=R[0][0];        P[0][1]=0;    P[0][2]=0;                           P[0][3]=0;
//            P[1][0]=0;        P[1][1]=f;    P[1][2]=0;                           P[1][3]=0;
//            P[2][0]=0;        P[2][1]=0;    P[2][2]=(zFar+zNear)/(zNear-zFar);   P[2][3]=(2*zFar*zNear)/(zNear-zFar);
//            P[3][0]=0;        P[3][1]=0;    P[3][2]=-1;                          P[3][3]=0;
//              return R4;
//        };
mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
    mat4 S4(1.0);
    S4[0][0]=sx;
    S4[1][1]=sy;
    S4[2][2]=sz;
    return S4;
};
mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
    mat4 T4(1.0);
    T4[0][3]=tx;
    T4[1][3]=ty;
    T4[2][3]=tz;
    return T4;
};
mat4 Transform::translate(const vec3 &vec_in)
{
    mat4 T4(1.0);
    T4[0][3]=vec_in[0];
    T4[1][3]=vec_in[1];
    T4[2][3]=vec_in[2];
    return T4;
};
