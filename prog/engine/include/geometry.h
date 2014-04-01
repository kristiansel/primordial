#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include <vector>
#include <iostream>

const unsigned int MAX_BONE_INFLUENCES = 4;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
//
const unsigned short normalOffset = 1*sizeof(glm::vec4);
const unsigned short texCoord0Offset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3);
const unsigned short bone_indexOffset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3)+2*sizeof(float);
const unsigned short bone_weightOffset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3)+2*sizeof(float)+MAX_BONE_INFLUENCES*sizeof(int);

//const unsigned short normalOffset = 1*sizeof(glm::vec4);
//const unsigned short texCoord0Offset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3);
//const unsigned short bone_indexOffset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3)+2*sizeof(GLfloat);
//const unsigned short bone_weightOffset = 1*sizeof(glm::vec4)+1*sizeof(glm::vec3)+2*sizeof(GLfloat)+MAX_BONE_INFLUENCES*sizeof(GLint);

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

//
//struct Vertex // 60 bytes
//{
////    Vertex();
////    static Vertex lerp(Vertex v1, Vertex v2);
////    static Vertex lerp(vector<Vertex> &verts_in);
//    glm::vec4 position; //X,Y,Z coordinates -----16bytes
//    glm::vec3 normal; //Vertex normals; -----12bytes
////    vec2 tex_coords; //Texture coordinates u, v -----8 bytes
//    GLfloat tex_coords[2];
//    GLint bone_indices[MAX_BONE_INFLUENCES]; // 4*4 = 16 bytes
//    GLfloat bone_weights[MAX_BONE_INFLUENCES]; // 4*4 = 16 bytes
//};
//
//struct Triangle // 12 bytes
//{
//    GLuint indices[3]; //Vertex indexes -----12 bytes                             12 bytes packed
////    unsigned short padding;
//};

// some overloads...
extern std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec);
extern std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec);
extern std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat);
extern std::ostream &operator<< (std::ostream &out, const glm::quat &quat);

#endif // GEOMETRY_H
