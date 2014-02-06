#ifndef SKELETON_H
#define SKELETON_H

#include <string>
#include <memory>
#include <list>
#include <unordered_map>
#include <glm/glm.hpp>

class Skeleton
{
public:
    class Bone;
    class Pose;
    class Animation;
public:
    Skeleton();
    virtual ~Skeleton();
protected:

private:
    std::shared_ptr<Bone> root_bone; /// Supports skeletons with one root bone for now

    std::unordered_map<std::string, std::shared_ptr<Animation>> animations; /// Unordered map is really useful, but is it performant?
    /// could consider to change the unordered map to a vector or array based on indices and enumerators if it proves too bad.
};

class Skeleton::Bone
{
public:
    struct Channels
    {
        /// Location/Translation    (Most often used for the root bone)
        float loc_x;           /// [0]
        float loc_y;           /// [1]
        float loc_z;           /// [2]

        /// Rotation
        float quat_rot_w;      /// [3]
        float quat_rot_x;      /// [4]
        float quat_rot_y;      /// [5]
        float quat_rot_z;      /// [6]

        /// Scale (Rarely used)
        float scale_x;         /// [7]
        float scale_y;         /// [8]
        float scale_z;         /// [9]
    };
public:
    Bone();
    virtual ~Bone();

    std::string getName();
protected:

private:
    std::list<std::shared_ptr<Bone>> child_bones; /// Variable number of child bones

    Channels tr_chnls; /// Transform channles

    std::string name;

    /// Positional information

};

class Skeleton::Pose
{

};


#endif // SKELETON_H
