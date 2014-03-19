#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "glm/glm.hpp"
#include <vector>
#include <iostream>

const unsigned int MAX_BONE_INFLUENCES = 4;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const unsigned short normalOffset = 1*sizeof(glm::vec4);
const unsigned short texCoord0Offset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3);
const unsigned short bone_indexOffset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3)+2*sizeof(float);
const unsigned short bone_weightOffset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3)+2*sizeof(float)+MAX_BONE_INFLUENCES*sizeof(int);

struct Vertex // 60 bytes
{
//    Vertex();
//    static Vertex lerp(Vertex v1, Vertex v2);
//    static Vertex lerp(vector<Vertex> &verts_in);
    glm::vec4 position; //X,Y,Z coordinates -----16bytes
    glm::vec3 normal; //Vertex normals; -----12bytes
//    vec2 tex_coords; //Texture coordinates u, v -----8 bytes
    float tex_coords[2];
    int bone_indices[MAX_BONE_INFLUENCES]; // 4*4 = 16 bytes
    float bone_weights[MAX_BONE_INFLUENCES]; // 4*4 = 16 bytes
};

struct Triangle // 12 bytes
{
    unsigned short indices[3]; //Vertex indexes -----12 bytes                             12 bytes packed
//    unsigned short padding;
};

// some overloads...
extern std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec);
extern std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec);
extern std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat);

#endif // GEOMETRY_H
