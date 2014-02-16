#include <string>
#include <vector>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include "include/skeleton.h"


/// Forward declaration of helper function
/// It is defined below animPackBin
void node_rec(aiNode* node,
              std::ofstream& file,
              std::vector<std::string> &bone_names,
              glm::mat4 parent_mat = glm::mat4(1.0));

int get_bone_index(std::string bone_name_in,
                   std::vector<std::string> &bone_names);

glm::mat4 aiMat2glmMat4(aiMatrix4x4 aiMat);

void printMat(glm::mat4 mat_in);

float* mat2floatArr(glm::mat4 mat_in);

/// Main function
bool animPackBin(std::string filepath, std::string outputpath)
{
    Assimp::Importer importer;

    const aiScene* scene;   /// Non-owner pointer

    scene = importer.ReadFile( filepath,
    aiProcess_CalcTangentSpace       |
    aiProcess_Triangulate            |
    aiProcess_JoinIdenticalVertices  |
    aiProcess_SortByPType);

    if( !scene) /// There were errors
    {
        std::cerr << importer.GetErrorString() << "\n";
        return false;
    }
    else /// import went smoothly
    {
        /// Open file for writing
        std::ofstream myFile (outputpath, std::ios::out | std::ios::binary);

        /// Write a file header
        char* filetype = "bbns";         /// 4 bytes     file type
        int verMaj = 1;                  /// 4 bytes     version major
        int verMin = 1;                  /// 4 bytes     version minor

        myFile.write(        filetype,    4*sizeof(char) );
        myFile.write( (char*) &verMaj,    1*sizeof(int) );
        myFile.write( (char*) &verMin,    1*sizeof(int) );

        /// Get the bone names from the animation channels
        std::vector<std::string> boneNames;

        for (int u = 0; u<scene->mNumAnimations; u++)
        {
            aiAnimation* animation = scene->mAnimations[u];
            for (int i = 0; i<animation->mNumChannels; i++)
            {
                aiNodeAnim* nodeAnim = animation->mChannels[i];
                std::string nodeName = nodeAnim->mNodeName.C_Str();

                if (std::find(boneNames.begin(), boneNames.end(), nodeName) == boneNames.end())
                {
                    boneNames.push_back(nodeName);
                }
            }
        }
        /// Now that the bone names have been found, write the number
        /// of bones
        int numBonesFound = boneNames.size();
        myFile.write( (char*) &numBonesFound,    1*sizeof(int) );

        /// recursively traverse the scene graph, save bones with name,
        /// index (should be in order) and matrix (same alignment as
        /// glm
        // here
        // multiply matrices as they go to get opengl coordinates
        // scene/root node contains the blender->opengl transformation
        node_rec(scene->mRootNode, myFile, boneNames);

        /// animation number, just all animations for now
        int numAnimations = scene->mNumAnimations;
        myFile.write( (char*) &numAnimations,    1*sizeof(int) );

        for (int i = 0; i<numAnimations; i++)
        {
            /// animation data, index and name
            aiAnimation* anim = scene->mAnimations[i];

            /// Write index
            myFile.write( (char*) &i,    1*sizeof(int) );

            /// Write duration
            // Ticks / ( ticks/second ) = seconds
            float duration = anim->mDuration / anim->mTicksPerSecond;
            myFile.write( (char*) &duration, 1*sizeof(float));

            /// Write number of channels
            int numChannels = anim->mNumChannels;
            myFile.write( (char*) &numChannels, 1*sizeof(int));

            for (int j = 0; j<numChannels; j++)
            {
                aiNodeAnim* channel = anim->mChannels[j];
                /// Write the bone index corresponding to this channel
                int index = get_bone_index(channel->mNodeName.C_Str(), boneNames);
                myFile.write ( (char*) &index, 1*sizeof(int));

                /// Position keys
                int numPosKeys = channel->mNumPositionKeys;
                myFile.write ( (char*) &numPosKeys, 1*sizeof(int));

                /// Write they keyframes, assume they are always in chron order
                for (int k = 0; k<numPosKeys; k++)
                {
                    /// Write time
                    aiVectorKey posKey = channel->mPositionKeys[k];
                    float time = posKey.mTime/anim->mTicksPerSecond;
                    myFile.write ( (char*) &time, 1*sizeof(float));

                    /// Write value
                    /// SHIT: THESE VALUES ARE NOT YET TRANSFORMED
                    /// TO OPENGL SPACE
                    aiVector3D posVec = posKey.mValue;
                    float posArr[3] = { posVec.x, posVec.y, posVec.z };
                    myFile.write ( (char*) &posArr[0], 3*sizeof(float));
                }

                /// Rotation keys
                int numRotKeys = channel->mNumRotationKeys;
                myFile.write ( (char*) &numRotKeys, 1*sizeof(int));

                /// Write they keyframes, assume they are always in chron order
                for (int k = 0; k<numRotKeys; k++)
                {
                    /// Write time
                    aiQuatKey rotKey = channel->mRotationKeys[k];
                    float time = rotKey.mTime/anim->mTicksPerSecond;
                    myFile.write ( (char*) &time, 1*sizeof(float));

                    /// Write value
                    /// SHIT: THESE VALUES ARE NOT YET TRANSFORMED
                    /// TO OPENGL SPACE
                    aiQuaternion rotQuat = rotKey.mValue;
                    float rotArr[4] = { rotQuat.w, rotQuat.x, rotQuat.y, rotQuat.z };
                    myFile.write ( (char*) &rotArr[0], 4*sizeof(float));
                }

                /// Scaling keys
                int numScaKeys = channel->mNumScalingKeys;
                myFile.write ( (char*) &numScaKeys, 1*sizeof(int));

                /// Write they keyframes, assume they are always in chron order
                for (int k = 0; k<numScaKeys; k++)
                {
                    /// Write time
                    aiVectorKey scaKey = channel->mScalingKeys[k];
                    float time = scaKey.mTime/anim->mTicksPerSecond;
                    myFile.write ( (char*) &time, 1*sizeof(float));

                    /// Write value
                    /// SHIT: THESE VALUES ARE NOT YET TRANSFORMED
                    /// TO OPENGL SPACE
                    aiVector3D scaVec = scaKey.mValue;
                    float scaArr[3] = { scaVec.x, scaVec.y, scaVec.z };
                    myFile.write ( (char*) &scaArr[0], 3*sizeof(float));
                }

            }

        }

        // here


        /// Close file
        myFile.close();
    }

    /// Importer deletes scene
}


/// Helper function for saving bones
void node_rec(aiNode* node,
              std::ofstream& file,
              std::vector<std::string> &bone_names,
              glm::mat4 parent_mat)
{
    int node_index = get_bone_index(node->mName.C_Str(), bone_names);

    glm::mat4 node_mat = aiMat2glmMat4(node->mTransformation);
    glm::mat4 abs_mat = parent_mat * node_mat;

    if (-1 != node_index) /// found an index
    {
        // Do if a bone
//        std::cout << "found bone " << node->mName.C_Str() << "\n";
        /// Write the bone index
        file.write( (char*) &node_index,    1*sizeof(int) );

        /// Write the length and contents of name
        std::string name = node->mName.C_Str();
        int nameLen = name.length();
        file.write( (char*) &nameLen,          1*sizeof(int) );
        file.write( (char*) &name[0],    nameLen*sizeof(char) ); // unsure

        /// Write the bone matrix
        float* matarr = mat2floatArr(abs_mat);;
        file.write( (char*) matarr,    16*sizeof(float) );
        delete [] matarr;

        /// Write the number of children
        int numChildren = node->mNumChildren;
        file.write( (char*) &numChildren,    1*sizeof(int) );

        /// Write the array of children
        int* child_indices = new int [numChildren];
        for (int i = 0; i<numChildren; i++)
        {
            child_indices[i] = get_bone_index(node->mChildren[i]->mName.C_Str(), bone_names);
        }
        file.write((char*) child_indices,  numChildren*sizeof(int));
        delete [] child_indices;
    }

    for (int i = 0; i<node->mNumChildren; i++)
    {
        node_rec(node->mChildren[i], file, bone_names, abs_mat);
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
    glm::mat4 glmMat;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            glmMat[i][j] = aiMat[i][j];
        }
    }
    return glmMat;
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
