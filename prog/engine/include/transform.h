#ifndef TRANS_H
#define TRANS_H


// Transform header file to define the interface.
// The class is all static for simplicity
// You need to implement left, up and lookAt
// Rotate is a helper function

// Include the helper glm library, including matrix transform extensions

//#include "3dtypes.h"
#include "quaternion.h"
#include <iostream>
#include <glm/glm.hpp>
#include <sstream>

using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;

// glm provides vector, matrix classes like glsl
// Typedefs to make code more readable


//const float pi = 3.14159265 ; // For portability across platforms

//class mat4 public glm::mat4 {
//    const ma4x4 operator * (mat4 rhs) {
//        mat4 out;
//        out[0][0] = this[0][0]
//
//
//
//    }
//}
class mm
{
public:
    mm() {};
    static mat4 matMult(mat4 m1, mat4 m2)
    {
        return glm::transpose(glm::transpose(m1)*glm::transpose(m2));
    }
    static void matPrint(mat4 m1)
    {
        for (int i = 0; i<4; i++)
        {
            for (int j = 0; j<4; j++)
            {
                std::cout<<m1[i][j]<<"\t";
            }
            std::cout<<"\n";
        }
        std::cout<<"\n";
    }
    static void vecPrint(vec3 v1)
    {
        for (int i = 0; i<3; i++)
        {
            std::cout<<v1[i]<<"\t";
        }
        std::cout<<"\n";
    }
    static void vecPrint(vec4 v1)
    {
        for (int i = 0; i<4; i++)
        {
            std::cout<<v1[i]<<"\t";
        }
        std::cout<<"\n";
    }
    static mat4 mat3_to_mat4(mat3 min)
    {
        mat4 mout;
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                mout[i][j] = min[i][j];
            }
        }
        mout[3][0] = 0;
        mout[3][1] = 0;
        mout[3][2] = 0;
        mout[3][3] = 1;
        mout[0][3] = 0;
        mout[1][3] = 0;
        mout[2][3] = 0;
        return mout;
    }
    static std::string stringVec3(vec3 in)
    {
        std::ostringstream out;
        out << "(" << in.x << ", " << in.y << ", " << in.z << ")";
        return out.str();
    }
};

class Transform
{
public:

    Transform();
    virtual ~Transform();
    static void left_cb(float degrees, vec3& eye, vec3& dir, vec3& up, vec3 point);
    static void up_cb(float degrees, vec3& eye, vec3& dir, vec3& up, vec3 point);
    static void left(float degrees, vec3& eye, vec3& up);
    static void up(float degrees, vec3& eye, vec3& up);
    static void forward(float speed, vec3& eye, vec3 &dir, vec3& up);
    static void strafe_left(float speed, vec3& eye, vec3 &dir, vec3& up);
    static mat4 quatRot(Quat local_rotation);
    static mat4 objectRot(vec3 up, vec3 dir);
    static mat4 lookAt(const vec3& eye, const vec3 &dir, const vec3& up);
    static mat4 perspective(float fovy, float aspect, float zNear, float zFar);
    static mat3 rotate(const float degrees, const vec3& axis) ;
//        static mat4 rotate(const float degrees, const vec3& axis) ;
    static mat4 scale(const float &sx, const float &sy, const float &sz) ;
    static mat4 translate(const float &tx, const float &ty, const float &tz);
    static mat4 translate(const vec3 &vec_in);
};

#endif // TRANS_H
