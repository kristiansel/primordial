#ifndef QUATERNION_H
#define QUATERNION_H



#include <cmath>
#include <iostream>
#include "glm/glm.hpp"

using glm::vec3;

struct Quat
{
public:
    Quat();
    Quat(float w, float x, float y, float z);
    Quat(vec3 axis, float angle_deg);
    float magnitude();
    void normalize();
    Quat ret_normalized();
    static Quat lerp(const Quat &q1, const Quat &q2, float t);
    static Quat slerp(const Quat &q1, const Quat &q2, float t);
    static float dot(const Quat &q1, const Quat &q2);
    void print()
    {
        std::cout<<w<<"\t"<<x<<"\t"<<y<<"\t"<<z<<"\n\n";
    }

    float & operator [] (int a)
    {
        if (a == 0)
        {
            return w;
        }
        if (a == 1)
        {
            return x;
        }
        if (a == 2)
        {
            return y;
        }
        if (a == 3)
        {
            return z;
        }
    }
    Quat &operator =(const Quat &q)
    {
        w=q.w;
        x=q.x;
        y=q.y;
        z=q.z;
        return *this;
    }

    const Quat operator +(const Quat &q) const
    {
        return Quat(w+q.w, x+q.x, y+q.y, z+q.z);
    }

    const Quat operator -(const Quat &q) const
    {
        return Quat(w-q.w, x-q.x, y-q.y, z-q.z);
    }
    const Quat operator /(const float &s) const
    {
        return Quat(w/s, x/s, y/s, z/s);
    }
    Quat operator * (const Quat in) const   // CHECK THIS
    {
        Quat out;
        out.w = w*in.w - x*in.x - y*in.y - z*in.z;
        out.x = w*in.x + x*in.w + y*in.z - z*in.y;
        out.y = w*in.y - x*in.z + y*in.w + z*in.x;
        out.z = w*in.z + x*in.y - y*in.x + z*in.w;
        return out;
    }

    const Quat operator *(float scale) const
    {
        return Quat(w*scale, x*scale, y*scale, z*scale);
    }



    float w, x, y, z;

    virtual ~Quat();
protected:
private:
};
#endif // QUATERNION_H
