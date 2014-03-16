#ifndef GEOPACKBIN_H
#define GEOPACKBIN_H

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <../../../engine/include/geometry.h> // For MAX_BONE_INFLUENCES
#include <animpackbin.hpp> // For get_bone_index(std::string bone_name_in, std::vector<std::string> &bone_names);




std::string int2str(int input) {
    // I can't believe it is this hard to convert 123 to "123"
    return static_cast<std::ostringstream*>( &(std::ostringstream() << input) )->str();
}

void find_rig_transf(aiNode* node,
              std::vector<std::string> &bone_names,
              glm::mat4 parent_mat,
              glm::mat4 &rig_transf,
              bool &rig_transf_found);

void absNodeTransfByName(aiNode *node,
                         std::string name,
                         glm::mat4 parent_mat,
                         glm::mat4 &result,
                         bool &found_yet);

// Should use index for mesh identification
void findAbsMeshTransf(std::string mesh_name, aiNode* node,
                            glm::mat4 &output_mat,
                            bool &found,
                            glm::mat4 parent_mat = glm::mat4(1.0));

void findAbsMeshTransf(int mesh_index, aiNode* node,
                            glm::mat4 &output_mat,
                            bool &found,
                            glm::mat4 parent_mat = glm::mat4(1.0));

void printMat(glm::mat4 mat_in, bool diffsig)
{
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            debugstream << ((mat_in[j][i] > 0.001 || mat_in[j][i] < -0.001) ? mat_in[j][i] : 0.0) << "\t";
//            debugstream << mat_in[j][i] << "\t";
        }
        debugstream << "\n";
    }
}

void nodePrint(aiNode* node, int max_lvl = 3, int lvl = 0)
{
    std::string tabs(lvl, '>');
    debugstream << tabs << "node: " << node->mName.C_Str() << "\n";
    debugstream << tabs << "numMeshes: " << node->mNumMeshes << "\n";
    debugstream << tabs << "numChildren: " << node->mNumChildren << "\n";
    debugstream << tabs << "transform: " << "\n";
    for (int i = 0; i<4; i++)
    {
        debugstream<<tabs;
        for (int j = 0; j<4; j++)
        {
            debugstream << node->mTransformation[i][j] << "\t";
        }
        debugstream << "\n";
    }

    if (lvl<max_lvl)
    {
        for(int i = 0; i<node->mNumChildren; i++)
            nodePrint(node->mChildren[i], max_lvl, lvl+1);
    }
}

void geoPackBin(const aiScene* scene, std::string outputpath, bool debug = true)
{
//    std::fstream debugstream;
    debugstream.open("geodebug.log", std::fstream::in | std::fstream::out | std::fstream::app);

    time_t ctt = time(0);
    debugstream << "=================================================\n";
    debugstream << "time: " << asctime(localtime(&ctt));

    debugstream << "-------------------------------------------------\n";
    debugstream << "dumping node hierarchy: \n";
    nodePrint(scene->mRootNode);
    debugstream << "-------------------------------------------------\n";

    debugstream << "exporting meshes...: \n";

    for (int i_mesh = 0; i_mesh<scene->mNumMeshes; i_mesh++)
    {
        // Bind the current mesh as an easy-to-use pointer;
        aiMesh* mesh = scene->mMeshes[i_mesh];

        debugstream << "exporting mesh: " << mesh->mName.C_Str() << "\n";

        // Disambiguate each output mesh filename
        outputpath = ( (0==i_mesh) ? outputpath : (outputpath + int2str(i_mesh)) );

        // Open file for writing
        std::ofstream myFile (outputpath, std::ios::out | std::ios::binary);


        // Write a file header
        char* filetype = "bgeo";         // 4 bytes     file type
        int verMaj = 1;                  // 4 bytes     version major
        int verMin = 1;                  // 4 bytes     version minor

        myFile.write(        filetype,    4*sizeof(char) );
        myFile.write( (char*) &verMaj,    1*sizeof(int) );
        myFile.write( (char*) &verMin,    1*sizeof(int) );

//        std::cout << "numBones: " << mesh->mNumBones << std::endl;


        // Number of vertices
        int numVerts = mesh->mNumVertices;
        myFile.write( (char*) &numVerts,    1*sizeof(int) );


        // Prepare bone indices in array
        // Get the bone names from the animation channels
        // This is necessary to use the same indexing as in
        // The animation exporter
        std::vector<std::string> boneNames = findBoneNames(scene);

        // Use the bone names to identify rig transform matrix (will later be applied
        // to written vertices

        // Find the mesh transform matrix
        glm::mat4 rig_transf(1.0);
        bool rig_transf_found = false;

        // The current method requires all meshes to be children directly of the root node (and have baked transforms...)
        if (mesh->HasBones())
        {
//            debugstream << "using parent skeleton transform" << std::endl;
//            find_rig_transf(scene->mRootNode, boneNames, glm::mat4(1.0), rig_transf, rig_transf_found);

            // new method: find rig transform based on bone offset matrix
            // find root bone of mesh

            // find a random bone:
            aiBone* a_bone = mesh->mBones[0];

            // offset = (model_transf * inverse(skel_transf) * from_skel_to_bone)
            glm::mat4 offset_mat = aiMat2glmMat4(a_bone->mOffsetMatrix);

            glm::mat4 from_mesh_to_bone = offset_mat;

            // find offset matrix from mesh to skeleton parent

                // find absolute parent matrix of root bone
                glm::mat4 abs_parent_root_bone(1.0);
                bool abs_found = false;
                find_rig_transf(scene->mRootNode, boneNames, glm::mat4(1.0), abs_parent_root_bone, abs_found);
                if(!abs_found) std::cerr << "could not find abs parent root mat\n";

                // find absolute matrix of "a_bone"
                glm::mat4 abs_a_bone_transf(1.0);
                bool abs_abone_found = false;
                absNodeTransfByName(scene->mRootNode, a_bone->mName.C_Str(), glm::mat4(1.0), abs_a_bone_transf, abs_abone_found);
                if(!abs_abone_found) std::cerr << "could not find abs a_bone mat\n";

//                // find absolute matrix of mesh
//                glm::mat4 abs_mesh_mat(1.0);
//                bool abs_mesh_found = false;
//                findAbsMeshTransf(i_mesh, scene->mRootNode, abs_mesh_mat, abs_mesh_found);

            glm::mat4 from_bone_to_skel = glm::inverse(abs_parent_root_bone) * abs_a_bone_transf;

//            glm::mat4 calc_abs_par_root_transf = glm::inverse(from_bone_to_skel) * offset_mat;

            glm::mat4 from_mesh_to_skel = from_bone_to_skel * from_mesh_to_bone;

            glm::mat4 from_skel_to_world = abs_parent_root_bone;

            glm::mat4 from_mesh_to_bind_world = from_skel_to_world * from_mesh_to_skel;

            rig_transf = from_mesh_to_bind_world;

            rig_transf_found = true;
//
//            debugstream << "abs_a_bone_transf\n";
//            printMat(abs_a_bone_transf, true);
//
//            debugstream << "from_bone_to_skel\n";
//            printMat(from_bone_to_skel, true);
//
//            debugstream << "from_mesh_to_bone\n";
//            printMat(from_mesh_to_bone, true);
//
////            debugstream << "abs_mesh_mat\n";
////            printMat(abs_mesh_mat, true);
//
//            debugstream << "from_mesh_to_skel\n";
//            printMat(from_mesh_to_skel, true);
//
//            debugstream << "from mesh coords to world (bind) coords\n";
//            printMat(from_mesh_to_bind_world, true);
//
//
//            debugstream << "skeleton_transform\n";
//            printMat(abs_parent_root_bone, true);

            // I bind in world coords
            // they bind in skel coords
        }
        else
        {
            debugstream << "using node hierarchy transform" << std::endl;
            findAbsMeshTransf(i_mesh, scene->mRootNode, rig_transf, rig_transf_found);
        }

//        debugstream << "using node hierarchy transform" << std::endl;
//        findAbsMeshTransf(i_mesh, scene->mRootNode, rig_transf, rig_transf_found);

        debugstream << "using rig_transf: \n";
        printMat(rig_transf, true);

        if (!rig_transf_found)
        {
            debugstream << "mesh transform not found... set to identity matrix\n";
        }

        std::vector<std::pair<int, float>>* bone_weight_pairs;
        bone_weight_pairs = new std::vector<std::pair<int, float>> [numVerts];


        for (int i_bone = 0; i_bone<mesh->mNumBones; i_bone++)
        {
            aiBone* bone = mesh->mBones[i_bone]; // They are not the same order!

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



        // Preform vertex bone-weight integrity check and post process
        for (int i_verts = 0; i_verts<numVerts; i_verts++)
        {
            auto bw_pair_arr = &(bone_weight_pairs[i_verts]);

            // Check that it has at least one bone
            if (bw_pair_arr->size()<1)  // if not, then add one
//                {
                    bw_pair_arr->push_back(std::make_pair((int)0, (float)1.0));
//                std::cerr << "warning: vertex without bone" << std::endl;}

            // Check that it has at least one non-zero weight
            bool ok = true;
            float sumWeights = 0.0;
            for (auto bw_pair : *bw_pair_arr)
            {
                sumWeights+=bw_pair.second;
            }

            // Integrity check is done

            // Now do the post-process
            // resize bone-weight vector to 4 and set new
            // element to 0: 0.0, then renormalize
            int oldSize = bw_pair_arr->size();
            bw_pair_arr->resize(MAX_BONE_INFLUENCES);

            for (int i_new = oldSize; i_new<MAX_BONE_INFLUENCES; i_new++)
            {
                // initialize the new bone_id-weight pair
                (*bw_pair_arr)[i_new] = std::make_pair((int)0, (float)0.0);
            }

            // renormalize
            sumWeights = 0.0;
            for (auto bw_pair : *bw_pair_arr) sumWeights+=bw_pair.second;
            for (auto &bw_pair : *bw_pair_arr) bw_pair.second=bw_pair.second/sumWeights; // Actually change

            // recheck
            sumWeights = 0.0;
            for (auto bw_pair : *bw_pair_arr) sumWeights+=bw_pair.second;

            if (std::abs(sumWeights)>1.01 || std::abs(sumWeights)<0.99)
                std::cerr << "error: vertex weights do not sum to 1"
                          << "mesh name: " << mesh->mName.C_Str() << "\n";
            // Post process finished
        }

        // mTextureCoords[i_verts] segfaults, probably because
        // there is one texcovecarray per triangle/face....
        // Write final vertices

        for (int i_verts = 0; i_verts<numVerts; i_verts++)
        {
            aiVector3D pos = mesh->mVertices[i_verts];
            aiVector3D norm = mesh->mNormals[i_verts];

            // Transform the pos and norm;
            glm::vec4 pos4 = glm::vec4(pos.x, pos.y, pos.z, 1.0);
            glm::vec4 norm4 = glm::vec4(norm.x, norm.y, norm.z, 0.0);

            pos4 = rig_transf * pos4;
            norm4 = glm::inverse( glm::transpose(rig_transf) ) * norm4;
//            norm4 = (rig_transf)* norm4;
            // two above ways of transforming normal are equivalent when
            // 4th component is 0...


            // Get just the first UV channel (hardcoded)
            aiVector3D txco;
            if (true==mesh->HasTextureCoords(0))
                txco = mesh->mTextureCoords[0][i_verts];
            else
                txco = aiVector3D(0.0, 0.0, 0.0);

            std::vector<std::pair<int, float>> bw_pairs = bone_weight_pairs[i_verts];

            // Flip
            txco[1] = 1.0-txco[1]; // <-- This

            // Write interleaved vertices
            myFile.write ( (char*) &pos4[0], 3*sizeof(float));
            myFile.write ( (char*) &norm4[0], 3*sizeof(float));
            myFile.write ( (char*) &txco[0], 2*sizeof(float));
            for (auto bw_pair : bw_pairs) myFile.write ( (char*) &bw_pair.first, 1*sizeof(int));
            for (auto bw_pair : bw_pairs) myFile.write ( (char*) &bw_pair.second, 1*sizeof(float));

        }

        // Write number of triangles
        myFile.write((char*) &(mesh->mNumFaces), 1*sizeof(unsigned int));

        // Assume triangulated (only faces of size 3)
        for (int i_tris = 0; i_tris<mesh->mNumFaces; i_tris++)
        {
            aiFace tri = mesh->mFaces[i_tris];
            if (tri.mNumIndices != 3) std::cerr << "error: found non-triangle\n";

            // Write indices
            unsigned short in0 = tri.mIndices[0];
            unsigned short in1 = tri.mIndices[1];
            unsigned short in2 = tri.mIndices[2];
            myFile.write ( (char*) &(in0), 1*sizeof(unsigned short));
            myFile.write ( (char*) &(in1), 1*sizeof(unsigned short));
            myFile.write ( (char*) &(in2), 1*sizeof(unsigned short));

//            std::cout << "writing tri: " << i_tris << "\n"
//                      << "   i(" << tri.mIndices[0] << ", " << tri.mIndices[1] << ", " << tri.mIndices[2] << ")\n";

            // Write texcoords ? or is there something wrong with this idea with
            // regard to shaders....?
//            mesh->mFaces.mIndices
        }

        delete[] bone_weight_pairs;

        // Close file
        myFile.close();
    }

    debugstream.close();
}

void find_rig_transf(aiNode* node,
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
            debugstream<<"parent node of rigged mesh: "<<node->mParent->mName.C_Str()<<std::endl;
            //printMat(rig_transf);
            return;
        }
    }

    if (!rig_transf_found)
    {
        for (int i = 0; i<node->mNumChildren; i++)
        {
            find_rig_transf(node->mChildren[i],
                     bone_names,
                     abs_mat,
                     rig_transf,
                     rig_transf_found);
        }
    }
}

void absNodeTransfByName(aiNode *node,
                         std::string name,
                         glm::mat4 parent_mat,
                         glm::mat4 &result,
                         bool &found_yet)
{
    glm::mat4 node_mat = aiMat2glmMat4(node->mTransformation);
    glm::mat4 abs_mat = parent_mat * node_mat;

    if (node->mName.C_Str() == name)
    {
        // found
        result = abs_mat;
        found_yet = true;
        debugstream<<"found transform of node "<<node->mName.C_Str()<<std::endl;
        return;
    }

    if (!found_yet)
    {
        for (int i = 0; i<node->mNumChildren; i++)
        {
            absNodeTransfByName(node->mChildren[i],
                                name,
                                abs_mat,
                                result,
                                found_yet);
        }
    }
}



//
void findAbsMeshTransf(std::string mesh_name, aiNode* node, // This is flawed at the moment
                            glm::mat4 &output_mat,
                            bool &found,
                            glm::mat4 parent_mat)
{
    glm::mat4 nodeRelTransf = aiMat2glmMat4(node->mTransformation);
    glm::mat4 nodeAbsTransf = parent_mat * nodeRelTransf;

//    std::cout<<"node rel transf for"<<node->mName.C_Str()<<"\n";
//    printMat(nodeAbsTransf);

    // Check if this node contains the mesh:
    if (std::string(node->mName.C_Str()) == mesh_name)
    // more robust here: search the node meshes array to look for the
    // index
    {
//            std::cout << "found transform\n";
        output_mat = nodeAbsTransf;
        found = true;
        return;
    }

    // If it got here, it was not this node:
    for (int i = 0; i<node->mNumChildren; i++)
    {
        findAbsMeshTransf(mesh_name, node->mChildren[i], output_mat, found, nodeAbsTransf);
    }
}

void findAbsMeshTransf(int mesh_index, aiNode* node, // This is flawed at the moment
                            glm::mat4 &output_mat,
                            bool &found,
                            glm::mat4 parent_mat)
{
    glm::mat4 nodeRelTransf = aiMat2glmMat4(node->mTransformation);
    glm::mat4 nodeAbsTransf = parent_mat * nodeRelTransf;

//    std::cout<<"node rel transf for"<<node->mName.C_Str()<<"\n";
//    printMat(nodeAbsTransf);

    // Check if this node contains the mesh:
    bool mesh_found_in_node = false;
    for (int i = 0; i<node->mNumMeshes; i++)
    {
        if (node->mMeshes[i] == mesh_index)
        {
            mesh_found_in_node = true;
        }
    }

    if (mesh_found_in_node)
    // more robust here: search the node meshes array to look for the
    // index
    {
//            std::cout << "found transform\n";
        output_mat = nodeAbsTransf;
        found = true;
        debugstream << "parent node to mesh: " << node->mParent->mName.C_Str() << "\n";
        return;
    }

    // If it got here, it was not this node:
    for (int i = 0; i<node->mNumChildren; i++)
    {
        findAbsMeshTransf(mesh_index, node->mChildren[i], output_mat, found, nodeAbsTransf);
    }
}
#endif // GEOPACKBIN_H
