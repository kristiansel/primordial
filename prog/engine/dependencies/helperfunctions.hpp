#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <sstream>
#include <glm/glm.hpp>


class helper
{
public:
    static glm::quat qlerp(const glm::quat &q1, const glm::quat &q2, float t)
    {
        glm::quat out;
        out = (q1*(1-t) + q2*t);
        out = glm::normalize(out); // Could be omitted?
        //std::cout<<"resorting to lerp\n";
        return out;
    //    return glm::normalize((q1*(1-t) + q2*t));
    }

    static glm::quat qslerp(glm::quat q1, glm::quat q2, float t)
    {
        float dot = glm::dot(q1, q2);

        if (dot < 0.0f) // IMPORTANT: Always choose this path
        {
            q1 = -q1; // Or q1 = -q1 (either should work)
            dot = -dot;
        }

    //    if (dot > -0.95f && dot < 0.95f)
    //    {
    //        float angle = acosf(dot);
    //        return (q1*sinf(angle*(1-t)) + q2*sinf(angle*t))/sinf(angle);
    //    }
    //    else    // if the angle is small, use linear interpolation
    //        return qlerp(q1,q2,t);
        float angle = acosf(dot);
        //return (dot > -0.95f && dot < 0.95f) ?
        return (std::abs(angle)>0.01) ?
                    (q1*sinf(angle*(1-t)) + q2*sinf(angle*t))/sinf(angle)
                    :   qlerp(q1,q2,t);
    }

};

#endif // HELPERFUNCTIONS_H
