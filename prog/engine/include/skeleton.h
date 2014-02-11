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
