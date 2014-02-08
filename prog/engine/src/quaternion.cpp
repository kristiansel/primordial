#include "quaternion.h"


// Use this in the transform code for bone rotations
Quat::Quat()
{
    //ctor
}

Quat::Quat(float w_in, float x_in, float y_in, float z_in)
{
    w = w_in;
    x = x_in;
    y = y_in;
    z = z_in;

};

Quat::Quat(vec3 axis, float angle_deg)
{
    float rad=3.14159265*angle_deg/180;
    float sinrad2 = sin(rad/2.0);
    w = cos(rad/2.0);
    x = axis.x*sinrad2;
    y = axis.y*sinrad2;
    z = axis.z*sinrad2;
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
