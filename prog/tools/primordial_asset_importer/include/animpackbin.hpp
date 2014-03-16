#ifndef ANIMPACKBIN_H
#define ANIMPACKBIN_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
//#include "include/skeleton.h"


// Forward declaration of helper function
// It is defined below animPackBin
void node_rec(aiNode* node,
              std::ofstream& file,
              std::vector<std::string> &bone_names,
              glm::mat4 parent_mat,
              glm::mat4 &rig_transf,
              bool &rig_transf_found);

int get_bone_index(std::string bone_name_in,
                   std::vector<std::string> &bone_names);

glm::mat4 aiMat2glmMat4(aiMatrix4x4 aiMat);

aiMatrix4x4 glm2aiMat4(glm::mat4 glmMat);

void printMat(glm::mat4 mat_in);

float* mat2floatArr(glm::mat4 mat_in);

std::vector<std::string> findBoneNames(const aiScene* scene);

void writeAnimation(const aiScene *scene,
                    unsigned int index,
                    std::ofstream &myFile,
                    std::vector<std::string> &boneNames,
                    std::fstream &debugstream,
                    bool debug);

std::fstream debugstream;

// Main function
bool animPackBin(const aiScene* scene, std::string outputpath, std::string animconfigpath = "", bool debug = true)
{
    // Open debug log file for writing
    std::ofstream myFile (outputpath, std::ios::out | std::ios::binary);

    debugstream.open("animdebug.log", std::fstream::in | std::fstream::out | std::fstream::app);

    time_t ctt = time(0);
    debugstream << "time: " << asctime(localtime(&ctt));


    // read the config "if present"
    std::ifstream configFile;
    configFile.open(animconfigpath);

    // get the directory of the animconfigpath
    std::string configDir = animconfigpath.substr(0, animconfigpath.find_last_of("/\\"));

//    std::cout << configDir << "\n";

    float duration_corrected = -1.0;
    bool use_config = configFile.is_open();
//
//    Assimp::Importer importer; // used later for importing additional animation files

    struct AniSpec {
        AniSpec() : specified(false) {};
        int anim_to_create;
        std::string anim_create_from;
        float time_start;
        float time_end;
        bool specified;
    };

    std::vector<AniSpec> anispecs;

    std::string line;
    while (std::getline(configFile, line))
    {
        AniSpec anispec;

        std::istringstream is(line);
        std::string anim, from, duration, to;

        is >> anim >> anispec.anim_to_create >> from >> anispec.anim_create_from
           >> duration >> anispec.time_start >> to >> anispec.time_end;

        // Need to prepend directory of the anispec file to the anispec.anim_create_from
        anispec.anim_create_from = configDir + "/" + anispec.anim_create_from;

        anispec.specified = true;

        anispecs.push_back(anispec);

//        std::cout << anispec.time_end << "\n";
    }

    // Write a file header
    char* filetype = "bbns";         // 4 bytes     file type
    int verMaj = 1;                  // 4 bytes     version major
    int verMin = 1;                  // 4 bytes     version minor

    myFile.write(        filetype,    4*sizeof(char) );
    myFile.write( (char*) &verMaj,    1*sizeof(int) );
    myFile.write( (char*) &verMin,    1*sizeof(int) );

    // Get the bone names from the animation channels
    std::vector<std::string> boneNames = findBoneNames(scene);

    for (int i_bone = 0; i_bone<boneNames.size(); i_bone++)
        debugstream << "bone " << i_bone << ": " << boneNames[i_bone] << std::endl;


    // Now that the bone names have been found, write the number
    // of bones
    int numBonesFound = boneNames.size();
    myFile.write( (char*) &numBonesFound,    1*sizeof(int) );

    // recursively traverse the scene graph, save bones with name,
    // index (should be in order) and matrix (same alignment as
    // glm
    // here
    // multiply matrices as they go to get opengl coordinates
    // scene/root node contains the blender->opengl transformation
    glm::mat4 rig_transf(1.0);
    bool found_rig_transf = false;
    node_rec(scene->mRootNode, myFile, boneNames, glm::mat4(1.0), rig_transf, found_rig_transf);

    // mat = trans * rot * sca
    // Here decompose the rig_transf matrix
    // Apply decomposed transform on all keyframes
    // Of the root bone

//        aiMatrix4x4 ai_rig_transf = glm2aiMat4(rig_transf);
    aiVector3D unused_pos;
    aiQuaternion unused_rot;
    aiVector3D unused_sca;
//
//        ai_rig_transf.Decompose(rig_sca, rig_rot, rig_pos); // Magic!

//        glm::vec3 rig_pos = vec3(ai_position.x, ai_position.y, ai_position.z);
//        glm::vec3 rig_rot = vec3(ai_rotation.w, ai_rotation.x, ai_rotation.y, ai_rotation.z);
//        glm::vec3 rig_sca = vec3(ai_scaling.x, ai_scaling.y, ai_scaling.z);


//        std::cout << "rig_transf: \n";
//        printMat(rig_transf);


    // animation number, just all animations for now
    int numAnimations = scene->mNumAnimations;
    if (use_config) numAnimations = anispecs.size();
    myFile.write( (char*) &numAnimations,    1*sizeof(int) );

    for (int i = 0; i<numAnimations; i++)
    {
        debugstream << "animation #: " << i << "\n";

        Assimp::Importer importer;

        if (use_config)
        {
            std::string ani_filename = anispecs[i].anim_create_from;

            std::cout << "creating animation " << i << " from file " << ani_filename << "\n";

            scene = importer.ReadFile( ani_filename,
                                    aiProcess_CalcTangentSpace       |
                                    aiProcess_Triangulate            |
                                    aiProcess_JoinIdenticalVertices  |
                                    aiProcess_SortByPType            |
                                    aiProcess_ImproveCacheLocality  );

            // Should assert that there are at least one animation
            if( !scene) // There were errors
            {
                std::cerr << importer.GetErrorString() << "\n";
                return false;
            }

            if (scene->mNumAnimations < 1)
            {
                std::cerr << "could not find animation in file " << ani_filename << "\n";
                return false;
            }
        }

        // animation data, index and name
        int scene_anim_index = i;
        if (use_config) scene_anim_index = 0;
        aiAnimation* anim = scene->mAnimations[scene_anim_index];

        // Write index
        myFile.write( (char*) &i,    1*sizeof(int) );

        // Write duration
        // Ticks / ( ticks/second ) = seconds
        float duration = anim->mDuration / anim->mTicksPerSecond;
        if (use_config) duration = anispecs[i].time_end-anispecs[i].time_start;
        myFile.write( (char*) &duration, 1*sizeof(float));


        debugstream << "#ticks: " << anim->mDuration << "\n";
        debugstream << "#ticks/sec: " << anim->mTicksPerSecond << "\n";
        debugstream << "duration: " << duration << "\n";

        // Write number of channels
        int numChannels = anim->mNumChannels;
        myFile.write( (char*) &numChannels, 1*sizeof(int));

        for (int j = 0; j<numChannels; j++)
        {
            aiNodeAnim* channel = anim->mChannels[j];
            // Write the bone index corresponding to this channel
            int index = get_bone_index(channel->mNodeName.C_Str(), boneNames);
            myFile.write ( (char*) &index, 1*sizeof(int));

//            if(debug) debugstream << "ch "<<index<<": " << channel->mNodeName.C_Str() << " ";

            // Position keys
            int numPosKeys = channel->mNumPositionKeys;
            myFile.write ( (char*) &numPosKeys, 1*sizeof(int));

//            if(debug) debugstream << "p(" << numPosKeys << "), ";

            // Write they keyframes, assume they are always in chron order
            for (int k = 0; k<numPosKeys; k++)
            {
                // Write time
                aiVectorKey posKey = channel->mPositionKeys[k];
                float time = posKey.mTime/anim->mTicksPerSecond;
                myFile.write ( (char*) &time, 1*sizeof(float));

                // Write value
                aiVector3D posVec = posKey.mValue;
                if (index==0)
                {
                    glm::vec4 pos_vec = glm::vec4(posVec.x, posVec.y, posVec.z, 1.0);
                    glm::vec4 pos_vec_rt = rig_transf * pos_vec;
                    posVec.x = pos_vec_rt.x;
                    posVec.y = pos_vec_rt.y;
                    posVec.z = pos_vec_rt.z;

                }
                float posArr[3] = { posVec.x, posVec.y, posVec.z };
                myFile.write ( (char*) &posArr[0], 3*sizeof(float));
            }

            // Rotation keys
            int numRotKeys = channel->mNumRotationKeys;
            myFile.write ( (char*) &numRotKeys, 1*sizeof(int));

//            if(debug) debugstream << "r(" << numRotKeys << "), ";

            // Write they keyframes, assume they are always in chron order
            for (int k = 0; k<numRotKeys; k++)
            {
                // Write time
                aiQuatKey rotKey = channel->mRotationKeys[k];
                float time = rotKey.mTime/anim->mTicksPerSecond;
                myFile.write ( (char*) &time, 1*sizeof(float));

                // Write value
                // SHIT: THESE VALUES ARE NOT YET TRANSFORMED
                // TO OPENGL SPACE
                aiQuaternion rotQuat = rotKey.mValue;
                if (index==0)
                {
                    glm::quat rot_quat = glm::quat(rotQuat.w,
                                                   rotQuat.x,
                                                   rotQuat.y,
                                                   rotQuat.z);
                    glm::mat4 rot_mat = glm::mat4_cast(rot_quat);
                    glm::mat4 rot_mat_rt = rig_transf * rot_mat;
                    aiMatrix4x4 aiMat = glm2aiMat4(rot_mat_rt);
                    aiMat.Decompose(unused_sca, rotQuat, unused_pos);
                }
                float rotArr[4] = { rotQuat.w, rotQuat.x, rotQuat.y, rotQuat.z };
                myFile.write ( (char*) &rotArr[0], 4*sizeof(float));
            }

            // Scaling keys
            int numScaKeys = channel->mNumScalingKeys;
            myFile.write ( (char*) &numScaKeys, 1*sizeof(int));

//            if(debug) debugstream << "s(" << numScaKeys << "), ";

            // Write they keyframes, assume they are always in chron order
            for (int k = 0; k<numScaKeys; k++)
            {
                // Write time
                aiVectorKey scaKey = channel->mScalingKeys[k];
                float time = scaKey.mTime/anim->mTicksPerSecond;
                myFile.write ( (char*) &time, 1*sizeof(float));

                // Write value
                // SHIT: THESE VALUES ARE NOT YET TRANSFORMED
                // TO OPENGL SPACE
                aiVector3D scaVec = scaKey.mValue;
                if (index==0)
                {
                    glm::vec3 sca_vec = glm::vec3(scaVec.x,
                                                  scaVec.y,
                                                  scaVec.z );
                    glm::mat4 sca_mat = glm::scale(glm::mat4(1.0),
                                                   sca_vec);
                    glm::mat4 sca_mat_rt = rig_transf * sca_mat;
                    aiMatrix4x4 aiMat = glm2aiMat4(sca_mat_rt);
                    aiMat.Decompose(scaVec, unused_rot, unused_pos);
                }
                float scaArr[3] = { scaVec.x, scaVec.y, scaVec.z };
                myFile.write ( (char*) &scaArr[0], 3*sizeof(float));
            }
//        if(debug) debugstream << std::endl;
        }  // Channels

    } // Animations


    // Close file
    myFile.close();

    debugstream << std::endl;
    debugstream.close();
}

// Helper function for saving bones
void node_rec(aiNode* node,
              std::ofstream& file,
              std::vector<std::string> &bone_names,
              glm::mat4 parent_mat,
              glm::mat4 &rig_transf,
              bool &rig_transf_found)
{
    int node_index = get_bone_index(node->mName.C_Str(), bone_names);

    glm::mat4 node_mat = aiMat2glmMat4(node->mTransformation);
    glm::mat4 abs_mat = parent_mat * node_mat;

    if (-1 != node_index) // found an index
    {
        if (!rig_transf_found)
        {
            rig_transf = parent_mat;
            rig_transf_found = true;
            debugstream << "skeleton parent node: " << node->mParent->mName.C_Str() << "\n";
        }
        // Do if a bone
//        std::cout << "found bone " << node->mName.C_Str() << "\n";
        // Write the bone index
        file.write( (char*) &node_index,    1*sizeof(int) );

        // Write the length and contents of name
        std::string name = node->mName.C_Str();
        int nameLen = name.length();
        file.write( (char*) &nameLen,          1*sizeof(int) );
        file.write( (char*) &name[0],    nameLen*sizeof(char) ); // unsure

        // Write the bone rest matrix
        float* matarr = mat2floatArr(abs_mat);;
        file.write( (char*) matarr,    16*sizeof(float) );
        delete [] matarr;

        // Write the number of bone children
        int numChildren = 0;
        for (int i = 0; i<node->mNumChildren; i++)
        {
            int childBoneIndex = get_bone_index(node->mChildren[i]->mName.C_Str(), bone_names);
            if (-1!=childBoneIndex) numChildren++;
        }

        file.write( (char*) &numChildren,    1*sizeof(int) );

        // Write the array of children
        int* child_indices = new int [numChildren];
        int boneChildCounter = 0;
        int nodeChildCounter = 0;
        while (nodeChildCounter<node->mNumChildren)
        {
            int childBoneIndex = get_bone_index(node->mChildren[nodeChildCounter]->mName.C_Str(), bone_names);
            if (-1!=childBoneIndex)
            {
                child_indices[boneChildCounter] = childBoneIndex;
                boneChildCounter++;
            }
            nodeChildCounter++;
        }

        file.write((char*) child_indices,  numChildren*sizeof(int));
        delete [] child_indices;
    }

    for (int i = 0; i<node->mNumChildren; i++)
    {
        node_rec(node->mChildren[i],
                 file, bone_names,
                 abs_mat,
                 rig_transf,
                 rig_transf_found);
    }
}

int get_bone_index(std::string bone_name_in,
                   std::vector<std::string> &bone_names)
{
    auto position = std::find(bone_names.begin(), bone_names.end(), bone_name_in);
    if (position != bone_names.end())
    {
        return std::distance(bone_names.begin(), position);
    }
    else
    {
        return -1;
    }
}

glm::mat4 aiMat2glmMat4(aiMatrix4x4 aiMat)
{
    // GLM matrix is COLUMN major (retarded)
    // 00 10 20 30
    // 01 11 21 31
    // 02 12 22 32
    // 03 13 23 33
    glm::mat4 glmMat;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            glmMat[j][i] = aiMat[i][j];
        }
    }
    return glmMat;
}

aiMatrix4x4 glm2aiMat4(glm::mat4 glmMat)
{
    aiMatrix4x4 aiMat;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            aiMat[j][i] = glmMat[i][j];
        }
    }
    return aiMat;
}

void printMat(glm::mat4 mat_in)
{
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            std::cout << mat_in[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

float* mat2floatArr(glm::mat4 mat_in)
{
    float * out = new float [16];
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            out[j+4*i] = mat_in[i][j]; // Store row major
        }
    }
    return out;
}

std::vector<std::string> findBoneNames(const aiScene* scene)
{
    std::vector<std::string> boneNamesOut;
    for (int u = 0; u<scene->mNumAnimations; u++)
    {
        aiAnimation* animation = scene->mAnimations[u];
        for (int i = 0; i<animation->mNumChannels; i++)
        {
            aiNodeAnim* nodeAnim = animation->mChannels[i];
            std::string nodeName = nodeAnim->mNodeName.C_Str();

            if (std::find(boneNamesOut.begin(), boneNamesOut.end(), nodeName) == boneNamesOut.end())
            {
                boneNamesOut.push_back(nodeName);
            }
        }
    }
    return boneNamesOut;
}

// Write animation i=index of the scene to file:
void writeAnimation(const aiScene *scene,
                    unsigned int i,
                    std::ofstream &myFile,
                    std::vector<std::string> &boneNames,
                    std::fstream &debugstream,
                    bool debug)
{


}


#endif
