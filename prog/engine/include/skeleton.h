#ifndef SKELETON_H
#define SKELETON_H

// GO DOWN FOR OLD IMPLEMENTATION
//     |
//     |
//     V

// PROPOSED NEW IMPLEMENTATION

#include <string>
#include <memory>
#include <list>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <fstream>
#include <iostream> // For debugging
#include <cstring>
#include <cmath>
#include <memstreamer.hpp>
#include <timeseries.hpp>
#include <sstream>
#include <helperfunctions.hpp>
#include <cstdlib>
#include <vector>



const int MAX_BONE_NUM = 100;

struct ActiveAnim
{
    int anim_index;
    float blend_weight;
    float anim_time;
    int uid;

    bool operator < (const ActiveAnim &right)
    {return blend_weight<right.blend_weight; };
};

class Skeleton // This is a resource (store one copy)
{
public:
    // Struct used for output interpolation output
    struct Pose;
public:
    Skeleton();
    virtual ~Skeleton();

    void fromFile(std::string skel_key);

    void poseMatrices(glm::mat4* matrices,
                      int anim_index,
                      float time,
                      int bone_index = 0,
                      glm::mat4 parent_mat = glm::mat4(1.0));

    void poseMatricesBlend(glm::mat4* matrices,
                          const std::vector<ActiveAnim> &active_anims,
                          int bone_index = 0,
                          glm::mat4 parent_mat = glm::mat4(1.0));

    int getNumBones() {return num_bones; };
    int getNumAnims() {return num_anims; };

    float getAnimDuration(int anim_index);
protected:

public: // private: // public for debugging
    // Some recursive function to apply bone transforms

    // Parts of the Skeleton structure
    class Bone;
    class Animation;


    // This does not use fancy STL because the data
    // will be sent to GPU

    int num_bones;
    Bone* bones;

    int num_anims;
    Animation* animations;

private: // for debuggin
    bool triggered;
};

class Skeleton::Bone // This is part of a resource (store one copy)
{
public:
    Bone() : rest_matrix(glm::mat4(1.0)),
             num_children(0),
             child_indices(nullptr){};

    ~Bone() {delete[] child_indices;};

    glm::mat4 rest_matrix; // Transform to move
                           // something from object coordinates
                           // to this bone's coordinates

    // Could perhaps in the future need to add a local
    // transform to be used when there are no key frames
    // if that is ever the case

    int num_children;
    int* child_indices;
    std::string name;


};

class Skeleton::Animation // This is part of a resource (store one copy)
{
public:
    class Channel; // Each channel corresponds to a bone
public:
    Animation();
    ~Animation();

    int num_channels;
    Channel* channels;

    float duration; // Seconds
};

/// Alias Skeleton::Animation as just Animation
//using Animation = Skeleton::Animation;

class Skeleton::Animation::Channel //
{
public:
    Channel();
    ~Channel();

    int bone_index; // Each channel corresponds to a bone

    TimeSeries<glm::vec3> pos_series;
    TimeSeries<glm::quat> rot_series;
    TimeSeries<glm::vec3> sca_series;

    // Helper vars
    float ch_duration;
    };

//// Output form data structure
//struct Skeleton::Pose
//{
//public:
//    struct Transform // Corresponding to bone/channel
//    {
//        Transform() : pos(glm::vec3(0.0, 0.0, 0.0)),
//                      rot(glm::quat(1.0, 0.0, 0.0, 0.0)),
//                      sca(glm::vec3(0.0, 0.0, 0.0)) {};
//        glm::vec3 pos;
//        glm::quat rot;
//        glm::vec3 sca;
//    };
//public:
//    Pose();
//    ~Pose();
//
//    explicit Pose(int num_transforms_in);
//    explicit Pose(Skeleton* skel);
//
//    int num_transforms;
//    Transform* transforms;
//
//private:
//    void allocate(int num_transforms_in);
//
//};



#endif // SKELETON_H
