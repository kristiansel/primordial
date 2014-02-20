#ifndef GEOPACKBIN_H
#define GEOPACKBIN_H

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <../../../engine/include/geometry.h> /// For MAX_BONE_INFLUENCES
#include <animpackbin.hpp> /// For get_bone_index(std::string bone_name_in, std::vector<std::string> &bone_names);
//
//struct BoneIDWeightPair
//{
//    BoneIDWeightPair : bone_id(0)
//                       weight(0.f) {};
//    int bone_id;
//    float weight;
//};

std::string int2str(int input) {
    /// I can't believe it is this hard to convert 123 to "123"
    return static_cast<std::ostringstream*>( &(std::ostringstream() << input) )->str();
}

void geoPackBin(const aiScene* scene, std::string outputpath)
{
    for (int i_mesh = 0; i_mesh<scene->mNumMeshes; i_mesh++)
    {
        /// Bind the current mesh as an easytouse pointer;
        aiMesh* mesh = scene->mMeshes[i_mesh];

        /// Disambiguate each output mesh
        outputpath = ( (0==i_mesh) ? outputpath : (outputpath + int2str(i_mesh)) );

        /// Open file for writing
        std::ofstream myFile (outputpath, std::ios::out | std::ios::binary);

        /// Write a file header
        char* filetype = "bgeo";         /// 4 bytes     file type
        int verMaj = 1;                  /// 4 bytes     version major
        int verMin = 1;                  /// 4 bytes     version minor

        myFile.write(        filetype,    4*sizeof(char) );
        myFile.write( (char*) &verMaj,    1*sizeof(int) );
        myFile.write( (char*) &verMin,    1*sizeof(int) );

        std::cout << "numBones: " << mesh->mNumBones << std::endl;


        /// Number of vertices
        int numVerts = mesh->mNumVertices;
        myFile.write( (char*) &numVerts,    1*sizeof(int) );


        /// Prepare bone indices in array
        /// Get the bone names from the animation channels
        /// This is necessary to use the same indexing as in
        /// The animation exporter
        std::vector<std::string> boneNames = findBoneNames(scene);

        std::vector<std::pair<int, float>>* bone_weight_pairs;
        bone_weight_pairs = new std::vector<std::pair<int, float>> [numVerts];

        for (int i_bone = 0; i_bone<mesh->mNumBones; i_bone++)
        {
            aiBone* bone = mesh->mBones[i_bone]; /// They are not the same order!

//            std::cout<< i_bone << " bone name: "<<bone->mName.C_Str()<<std::endl;

            for (int i_weights = 0; i_weights<bone->mNumWeights; i_weights++)
            {
                aiVertexWeight vWeight = bone->mWeights[i_weights];
                bone_weight_pairs[vWeight.mVertexId].push_back(
                    std::make_pair(get_bone_index(bone->mName.C_Str(),
                                                  boneNames),
                                   vWeight.mWeight));
            }
        }

        /// Check if any vertices do not have weights


        int** bone_indices_arr = new int* [numVerts];

        for(int i_verts=0; i_verts<numVerts; i_verts++)
        {
            bone_indices_arr[i_verts] = new int [MAX_BONE_INFLUENCES];
            for(int i_bone_inf=0; i_bone_inf<MAX_BONE_INFLUENCES; i_bone_inf++)
                bone_indices_arr[i_verts][i_bone_inf] = 0; /// Initialize to 0;
        }

        float** bone_weights_arr = new float* [numVerts];

        for(int i_verts=0; i_verts<numVerts; i_verts++)
        {
            bone_weights_arr[i_verts] = new float [MAX_BONE_INFLUENCES];
            for(int i_bone_wgt=0; i_bone_wgt<MAX_BONE_INFLUENCES; i_bone_wgt++)
                bone_weights_arr[i_verts][i_bone_wgt] = 0; /// Initialize to 0.0;
        }


        delete[] bone_weight_pairs;


        /// Before writing vertices (interleaved data of position,
        /// normal, texture coordinates and bone weights) the bone
        /// weights must be prepared in a corresponding array

        for (int i_verts = 0; i_verts<numVerts; i_verts++)
        {
            /// Write interleaved vertices

        }

        for(int i_verts=0; i_verts<numVerts; i_verts++)
            delete[] bone_weights_arr[i_verts];

        delete[] bone_weights_arr;


        for(int i_verts=0; i_verts<numVerts; i_verts++)
            delete[] bone_indices_arr[i_verts];

        delete[] bone_indices_arr;


        /// Close file
        myFile.close();
    }
}

#endif // GEOPACKBIN_H
