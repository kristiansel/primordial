#include "quaternion.h"


// Use this in the transform code for bone rotations
Quat::Quat()
{
    //ctor
}

Quat::Quat(float w_in, float x_in, float y_in, float z_in)
{
//    w = w_in;
//    x = x_in;
//    y = y_in;
//    z = z_in;
    fromWXYZ(w_in, x_in, y_in, z_in);

};

Quat::Quat(vec3 axis, float angle_deg)
{
//    float rad=3.14159265*angle_deg/180.f;
//    float sinrad2 = sin(rad/2.0);
//    w = cos(rad/2.0);
//    x = axis.x*sinrad2;
//    y = axis.y*sinrad2;
//    z = axis.z*sinrad2;
    fromAxisAngle(axis, angle_deg);
}

Quat::Quat(vec3 dir, vec3 ref_dir) /// get quaternion from direction
{
//    std::cout << "dir(" << dir.x << ", " << dir.y << ", " << dir.z << ")\n";
//    std::cout << "ref(" << ref_dir.x << ", " << ref_dir.y << ", " << ref_dir.z << ")\n";
//    const float myEPS = 1e-5;
//    /// Check if not the same
//    if (abs(dir.x-ref_dir.x)<myEPS && abs(dir.y-ref_dir.y)<myEPS && abs(dir.z-ref_dir.z)<myEPS) /// Parallel
//    {
//        std::cout << "sq1(" << w << ", " << x << ", " << y << ", " << z << ")\n";
//        Quat(1.0, 0.0, 0.0, 0.0);   /// Make it zero rotation (identity quaternion)
//        std::cout << "sq2(" << w << ", " << x << ", " << y << ", " << z << ")\n";
//        std::cout<<"got here\n";
//    }
//    else
//    {
//        dir = glm::normalize(dir);
//        dir = glm::normalize(ref_dir);
//
//        vec3 axis = glm::normalize(glm::cross(dir, ref_dir));
//        float angle_deg = 180.f*acos(glm::dot(dir, ref_dir))/3.14159265f;     /// no need to div by length since 1
//        Quat(axis, angle_deg);  /// Apply axis angle
//
//        // std::cout << "sq(" << w << ", " << x << ", " << y << ", " << z << ")\n";
//
//        std::cout<<"got here2\n";
//    }
    fromDirAndRef(dir, ref_dir);
}

void Quat::fromWXYZ(float w_in, float x_in, float y_in, float z_in)
{
    w = w_in;
    x = x_in;
    y = y_in;
    z = z_in;
};

void Quat::fromAxisAngle(vec3 axis, float angle_deg)
{
    float rad=3.14159265*angle_deg/180.f;
    float sinrad2 = sin(rad/2.0);
    w = cos(rad/2.0);
    x = axis.x*sinrad2;
    y = axis.y*sinrad2;
    z = axis.z*sinrad2;
}

void Quat::fromDirAndRef(vec3 dir, vec3 ref_dir)
{

    const float myEPS = 1e-5;
    /// Check if not the same
    if (abs(dir.x-ref_dir.x)<myEPS && abs(dir.y-ref_dir.y)<myEPS && abs(dir.z-ref_dir.z)<myEPS) /// Parallel
    {
        //std::cout << "sq1(" << w << ", " << x << ", " << y << ", " << z << ")\n";
        fromWXYZ(1.0, 0.0, 0.0, 0.0);   /// Make it zero rotation (identity quaternion)
        //std::cout << "sq2(" << w << ", " << x << ", " << y << ", " << z << ")\n";
        //std::cout<<"got here\n";
    }
    else
    {
        dir = glm::normalize(dir);
        ref_dir = glm::normalize(ref_dir);

//    std::cout << "dir(" << dir.x << ", " << dir.y << ", " << dir.z << ")\n";
//    std::cout << "ref(" << ref_dir.x << ", " << ref_dir.y << ", " << ref_dir.z << ")\n";

        vec3 axis = glm::normalize(glm::cross(dir, ref_dir));
        float angle_deg = 180.f*acos(glm::dot(dir, ref_dir))/3.14159265f;     /// no need to div by length since 1

//        std::cout << "axis(" << axis.x << ", " << axis.y << ", " << axis.z << ", " << 0 << ")\n";
//        std::cout << "angle("<<angle_deg<<")\n";

        fromAxisAngle(axis, angle_deg);  /// Apply axis angle

        // std::cout << "sq(" << w << ", " << x << ", " << y << ", " << z << ")\n";

//       std::cout<<"got here2\n";
    }
}

float Quat::magnitude()
{
    return sqrt(w*w+x*x+y*y+z*z);
}
void Quat::normalize()
{
    float mag = magnitude();
    w = w/mag;
    x = x/mag;
    y = y/mag;
    z = z/mag;
};
Quat Quat::ret_normalized()
{
    float mag = magnitude();
    return Quat(w/mag, x/mag, y/mag, z/mag);
};

float Quat::dot(const Quat &q1, const Quat &q2)
{
    return (q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z);
};

Quat Quat::lerp(const Quat &q1, const Quat &q2, float t)
{
    Quat out;
    out = (q1*(1-t) + q2*t);
    out.normalize();
    return out;
}

Quat Quat::slerp(const Quat &q1, const Quat &q2, float t)
{
    float dot = Quat::dot(q1, q2);

    if (dot > -0.95f && dot < 0.95f)
    {
        float angle = acosf(dot);
        return (q1*sinf(angle*(1-t)) + q2*sinf(angle*t))/sinf(angle);
    }
    else    // if the angle is small, use linear interpolation
        return lerp(q1,q2,t);
}


Quat::~Quat()
{
    //dtor
}
