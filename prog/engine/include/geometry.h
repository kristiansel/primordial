#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "GL/glm.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace glm;

const unsigned int MAX_BONE_INFLUENCES = 3;

struct Vertex // 60 bytes
{
//    Vertex();
//    static Vertex lerp(Vertex v1, Vertex v2);
//    static Vertex lerp(vector<Vertex> &verts_in);
    vec4 position; //X,Y,Z coordinates -----16bytes
    vec3 normal; //Vertex normals; -----12bytes
//    vec2 tex_coords; //Texture coordinates u, v -----8 bytes
    float tex_coords[2];
    int bone_indices[MAX_BONE_INFLUENCES]; // 3*4 = 12 bytes
    float bone_weights[MAX_BONE_INFLUENCES]; // 3*4 = 12 bytes
};

struct Triangle // 12 bytes
{
    unsigned short indices[3]; //Vertex indexes -----12 bytes                             12 bytes packed
//    unsigned short padding;
};


#endif // GEOMETRY_H
