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

class Skeleton /// This is a resource (store one copy)
{
public:
    /// Struct used for output interpolation output
    struct Pose;
public:
    Skeleton();
    virtual ~Skeleton();

    void fromFile(std::string filepath);

    int getNumBones() {return num_bones; };
protected:

private:
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
    Bone() {};
    ~Bone() {};

    glm::mat4 getRestMatrix() {return rest_matrix;};
private:
    glm::mat4 rest_matrix; /// Transform to move
                           /// something from parent origin
                           /// To this origin and orientation
                           /// in "rest"/"bind" pose
};

class Skeleton::Animation /// This is part of a resource (store one copy)
{
public:
    class Channel; /// Each channel corresponds to a bone
public:
    Animation();
    ~Animation();

    int getNumChannels() {return num_channels;};

    float getStartTime() {return start_time;};
    float getEndTime() {return end_time;};

private:
    int num_channels;
    Channel* channels;

    float start_time; /// Seconds
    float end_time; /// Seconds
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

    int getBoneIndex() {return bone_index;};

    int getNumPosKeys() {return num_pos_keys; };
    int getNumRotKeys() {return num_rot_keys; };
    int getNumScaKeys() {return num_sca_keys; };
protected:
    int bone_index; /// Each channel corresponds to a bone

    int num_pos_keys;
    int num_rot_keys;
    int num_sca_keys;

    TimePosPair* pos_keys;
    TimeRotPair* rot_keys;
    TimeScaPair* sca_keys;
};

/// Output form data structure
//struct Skeleton::Pose
//{
//public:
//    struct Transform /// Corresponding to bone/channel
//    {
//        glm::vec3 pos;
//        glm::quat rot;
//        glm::vec3 sca;
//    };
//public:
//    Pose();
//    ~Pose();
//
//    explicit Pose(int num_transforms);
//    explicit Pose(Skeleton* skel);
//
//    num_transforms;
//    Transform* transforms;
//
//};

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
