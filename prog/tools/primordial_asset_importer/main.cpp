#include <iostream>
#include <string>
#include <fstream>

#include "converter.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/// FROM THIS STUDY:
/// It seems as though collada/assimp stores the relative matrix to parent both in rest
/// and key framed poses. I guess this is actually less work than what I had in mind...
/// Should start working on an assimp -> binary tool



//using namespace std;
//
//float toZeroRound(float in)
//{
//    if (in < 1e-5 && in > -1e-5) return 0; else return in;
//}
//
//void writeMatrix(aiMatrix4x4 mat, ofstream& stream, string tabs)
//{
//    stream << tabs << "{" ;
//    for (int k = 0; k<4; k++)
//    {
//        stream << "{";
//        for (int j = 0; j<4; j++)
//        {
//            stream << toZeroRound(mat[k][j]);
//            if (j!=3) stream << ",";
//        }
//        stream << "}";
//        if (k!=3) stream << ",\n" << tabs;
//    }
//    stream << "}" << "\n";
//}
//
//
//void node_rec(aiNode* node, ofstream& file, int lvl)
//{
//
//    string tabs(lvl, '   ');
//    file << tabs << " : " <<node->mName.C_Str() << "\n";
//    file << tabs << " : num_children : " <<node->mNumChildren << "\n";
//    file << tabs << " : num_meshes : " <<node->mNumMeshes << "\n";
//    file << tabs << " : transformation :\n";
//
//    writeMatrix(node->mTransformation, file, tabs + "   ");
//
//
//    for (int i = 0; i<node->mNumChildren; i++)
//    {
//        node_rec(node->mChildren[i], file, lvl+1);
//    }
//}


int main()
{
//    ofstream myFile;
//    myFile.open("output.txt");
//
//    string pFile = "test_scenes/asset_anim_test5.dae";
//
//    Assimp::Importer importer;
//
//    const aiScene* scene;   /// Non-owner pointer
//
//    scene = importer.ReadFile( pFile,
//    aiProcess_CalcTangentSpace       |
//    aiProcess_Triangulate            |
//    aiProcess_JoinIdenticalVertices  |
//    aiProcess_SortByPType);
//
//    // If the import failed, report it
//    if( !scene)
//    {
//        cerr << importer.GetErrorString() << "\n";
//    }
//    else
//    {
////        myFile << "mRootNode = " << scene->mRootNode->mName.C_Str() << "\n";
//
//        node_rec(scene->mRootNode, myFile, 0);
//
//
//        myFile << "HasAnimations = " << scene->HasAnimations() << "\n";
//        myFile << "mNumAnimations = " << scene->mNumAnimations << "\n";
//        aiAnimation* animation = scene->mAnimations[0];
//        myFile << "animation duration = " << animation->mDuration << " s \n";
//        myFile << "animation name = " << animation->mName.C_Str() << " \n";
//        myFile << "numBoneChannels = " << animation->mNumChannels << " \n";
//        myFile << "numMeshChannels = " << animation->mNumMeshChannels << " \n";
//        myFile << "numTicksPerSecond = " << animation->mTicksPerSecond << " \n";
//        myFile << "ANIM_CHANNELS = " << animation->mTicksPerSecond << " \n";
//        for (int i = 0; i<animation->mNumChannels; i++)
//        {
//            myFile << "   CHANNEL = " << i << " \n";
//            aiNodeAnim* nodeAnim = animation->mChannels[i];
//            myFile << "   name = " << nodeAnim->mNodeName.C_Str() << " \n";
//            myFile << "   numPositionKeys = " << nodeAnim->mNumPositionKeys << " \n";
//            myFile << "   numRotationKeys = " << nodeAnim->mNumRotationKeys << " \n";
//            myFile << "   numScalingKeys = " << nodeAnim->mNumScalingKeys << " \n";
//            myFile << "   PostState = " << nodeAnim->mPostState << " \n";
//            for (int j = 0; j<nodeAnim->mNumPositionKeys; j++)
//            {
//                myFile << "       PosKeys = " << j << " \n";
//                aiVectorKey vecKey = nodeAnim->mPositionKeys[j];
//                double time = vecKey.mTime;
//                myFile << "       mTime = " << time << " \n";
//                aiVector3D keyValue = vecKey.mValue;
//                myFile << "       value = {" << keyValue[0] << ", " << keyValue[1] << ", " << keyValue[2] << "}\n";
//            }
//            for (int k = 0; k<nodeAnim->mNumRotationKeys; k++)
//            {
//                myFile << "       RotKeys = " << k << " \n";
//                aiQuatKey quatKey = nodeAnim->mRotationKeys[k];
//                double time =  quatKey.mTime;
//                myFile << "       mTime = " << time << " \n";
//                aiQuaternion keyValue = quatKey.mValue;
//                myFile << "       value = {" << keyValue.w << ", " << keyValue.x << ", " << keyValue.y << ", " << keyValue.z << "}\n";
//            }
//            for (int l = 0; l<nodeAnim->mNumScalingKeys; l++)
//            {
//                myFile << "       ScaleKeys = " << l << " \n";
//                aiVectorKey vecKey = nodeAnim->mScalingKeys[l];
//                double time = vecKey.mTime;
//                myFile << "       mTime = " << time << " \n";
//            }
//        }
//        for (int i = 0; i<scene->mNumMeshes; i++)
//        {
//            aiMesh* mesh = scene->mMeshes[i];
//            myFile << mesh->mName.C_Str() << "\n";
//            for (int l = 0; l<mesh->mNumBones; l++)
//            {
//                aiBone* bone = mesh->mBones[l];
//                myFile << "   " << bone->mName.C_Str() << "\n";
//                string tabs = "   ";
//                writeMatrix(bone->mOffsetMatrix, myFile, tabs+"  ");
//            }
//        }
//
//    }
//
//
//    myFile.close();
//
//
//
//    /// delete scene; // IMPORTER deletes the scene for us

//    animPackBin(pFile, "../../engine/assets_bin/skeletons/anim_test.bbns"); /// Binary Bones file

    convert("test_scenes/asset_anim_test6.dae",
            "../../engine/assets_bin/models/anim_test.bgeo",
            "../../engine/assets_bin/skeletons/anim_test.bbns");

    return 0;
}
