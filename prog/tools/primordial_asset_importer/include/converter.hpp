#ifndef CONVERTER_H
#define CONVERTER_H


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include "animpackbin.hpp"
#include "geopackbin.hpp"

bool convert(std::string daeFilePath_in,
             std::string geoFilePath_out,
             std::string bnsFilePath_out)
{

    Assimp::Importer importer;

    const aiScene* scene;   /// Non-owner pointer

//    scene = importer.ReadFile( daeFilePath_in,
//    aiProcess_CalcTangentSpace       |
//    aiProcess_Triangulate            |
//    aiProcess_JoinIdenticalVertices  |
//    aiProcess_SortByPType            |
//    aiProcess_PreTransformVertices);

    scene = importer.ReadFile( daeFilePath_in,
    aiProcess_CalcTangentSpace       |
    aiProcess_Triangulate            |
    aiProcess_JoinIdenticalVertices  |
    aiProcess_SortByPType            |
    aiProcess_ImproveCacheLocality);

    if( !scene) /// There were errors
    {
        std::cerr << importer.GetErrorString() << "\n";
        return false;
    }
    else /// import went smoothly
    {
        animPackBin(scene, bnsFilePath_out);
        geoPackBin(scene, geoFilePath_out);
    }

    /// Importer deletes scene
}
#endif // CONVERTER_H
