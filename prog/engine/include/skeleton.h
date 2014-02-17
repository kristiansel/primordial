#ifndef SKELETON_H
#define SKELETON_H

/// GO DOWN FOR OLD IMPLEMENTATION
///     |
///     |
///     V

/// PROPOSED NEW IMPLEMENTATION

#include <string>
#include <memory>
#include <list>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <fstream>
#include <iostream> /// For debugging
#include <cstring>

class Skeleton /// This is a resource (store one copy)
{
public:
    /// Struct used for output interpolation output
    struct Pose;
public:
    Skeleton();
    virtual ~Skeleton();

    void fromFile(std::string filepath);
    void poseMatrices(glm::mat4* matrices,
                      int anim_index,
                      float time,
                      int bone_index = 0,
                      glm::mat4 parent_mat = glm::mat4(1.0));

    int getNumBones() {return num_bones; };
protected:

public: /// private: // public for debugging
    /// Some recursive function to apply bone transforms

    /// Parts of the Skeleton structure
    class Bone;
    class Animation;


    /// This does not use fancy STL because the data
    /// will be sent to GPU

    int num_bones;
    Bone* bones;

    int num_anims;
    Animation* animations;
};

class Skeleton::Bone /// This is part of a resource (store one copy)
{
public:
    Bone() : rest_matrix(glm::mat4(1.0)),
             num_children(0),
             child_indices(nullptr){};

    ~Bone() {delete[] child_indices;};

    glm::mat4 rest_matrix; /// Transform to move
                           /// something from parent origin
                           /// To this origin and orientation
                           /// in "rest"/"bind" pose
    int num_children;
    int* child_indices;
    std::string name;


};

class Skeleton::Animation /// This is part of a resource (store one copy)
{
public:
    class Channel; /// Each channel corresponds to a bone
public:
    Animation();
    ~Animation();

    int num_channels;
    Channel* channels;

    float duration; /// Seconds

};

class Skeleton::Animation::Channel ///
{
public:
    struct TimePosPair {float time; glm::vec3 pos; }; /// Consider making argument-less constructor private
    struct TimeRotPair {float time; glm::quat rot; };
    struct TimeScaPair {float time; glm::vec3 sca; };
public:
    Channel();
    ~Channel();

    int bone_index; /// Each channel corresponds to a bone

    int num_pos_keys;
    int num_rot_keys;
    int num_sca_keys;

    TimePosPair* pos_keys;
    TimeRotPair* rot_keys;
    TimeScaPair* sca_keys;
};

/// Output form data structure
struct Skeleton::Pose
{
public:
    struct Transform /// Corresponding to bone/channel
    {
        Transform() : pos(glm::vec3(0.0, 0.0, 0.0)),
                      rot(glm::quat(1.0, 0.0, 0.0, 0.0)),
                      sca(glm::vec3(0.0, 0.0, 0.0)) {};
        glm::vec3 pos;
        glm::quat rot;
        glm::vec3 sca;
    };
public:
    Pose();
    ~Pose();

    explicit Pose(int num_transforms_in);
    explicit Pose(Skeleton* skel);

    int num_transforms;
    Transform* transforms;

private:
    void allocate(int num_transforms_in);

};

class MemFuncOb /// This could be useful for anything that reads from memory blob
{
public:
    MemFuncOb(const char * memblock_in, int offset_in, int memsize_in)
             : memblock(memblock_in),
               offset(offset_in),
               memsize(memsize_in) {};
    ~MemFuncOb() {};

    void chomp (void * dest, size_t size_in)
    {
        if (offset < memsize || size_in == size_t(0))
        {
            memcpy(dest, &memblock[offset], size_in);
            offset += size_in/sizeof(char);
        }
        else
        {
            std::cerr << "Error: tried to read outside memory block\n";
        }

    }
private:
    const char * memblock;
    int offset;
    int memsize;
};

/// END PROPOSED NEW IMPLEMENTATION
//
//#include "glm/glm.hpp"
//#include "quaternion.h"
//#include <string>
//#include <vector>
//#include <fstream>
//#include <string.h>
//#include <stdlib.h>
//#include "transform.h"
//
////#include "graphics.h"
////#include "consts.h"
//
//using std::string;
//using std::vector;
//using std::cerr;
//using std::abs;
//using std::ifstream;
//// using std::strtok;
//
//class Animation
//{
//public:
//    const static int ANIMS_NUM = 4;
//    enum Anum
//    {
//        STAND,
//        WALK,
//        RUN,
//        CROUCH
//    };
//
//    static const string anim_name[];
//
//
//    Animation();
//    Animation(string name_in, float start, float end);
//    virtual ~Animation();
//
//    int start_Frame;
//    int end_Frame;
//    string name;
//    int number;
//
//protected:
//private:
//};
//
//struct KeyFrame
//{
//    KeyFrame();
//    KeyFrame(float frame_in, float value_in);
//    int frame;
//    float value;
//};
//
//class Bone
//{
//public:
//    Bone();
//    virtual ~Bone();
//
//    const static int MAX_BONE_CHILDREN = 8;
//
//    int index;
//    int parentIndex;
//    int numChildren;
//    int childIndex[MAX_BONE_CHILDREN];
//
//    vec3 head;
//    vec3 tail;
//
//    mat4 rest_matrix;
//    mat4 rest_del_mat;
//
//    vector<KeyFrame> keyPositions[10]; //
//
//    string name;
//
//protected:
//private:
//};
//
//class Skeleton
//{
//public:
//    Skeleton();
//    virtual ~Skeleton();
//
//    static const float DEBUG_FRAME;
//
//    void loadFromBNS(std::string file_path);
//    void addAnim(Animation anim_in);
//
////        void drawRestPose(mat4 mv);
////        void drawFramePose(mat4 mv, float frame);
//
//    //void interPolateTo(float frame);
//    void advance_Interpolation(float dt);
//    void jump2frame(float frame);
//    void startAnimation(int num);
//    void findRightInd();
//    void rec_bone_matrices(int ind);
//    void rec_bone_matrices_rest(int ind, vector<mat4> &temp_matrices);
//    void rec_bone_matrices_rest_rolls(int ind, vector<float> &rolls);
//
//    int numBones;
//    vector<Bone> bones_rest;
//    vector<Animation> animations;
//    int root_index;
//
//    vector<mat4> bone_matrices;
//    vector<mat4> norm_matrices;
//    vector<mat4> bone_delta_mats;
//    vector<mat4> Ds; // helper storage
//    vector<Quat> slerped_rots;
//    vector<vec3> lerped_posits;
//
//    // Internal running variables that govern interpolation;
//    float currentFrame;
//    int leftKeyIndex;
//    int rightKeyIndex;
//    int ActiveAnimationIndex;
//
//    float min_frame;
//    float max_frame;
//
//    //debugging
//    bool fired;
//
//
//protected:
//private:
//};


#endif // SKELETON_H
