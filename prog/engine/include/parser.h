#ifndef PARSER_H
#define PARSER_H

# include "geometry.h"
# include "skeleton.h"
# include <GL/gl.h>
# include <fstream>
# include <string.h>
# include <stdlib.h>

using std::string;
using std::vector;
using std::ifstream;
using std::cerr;

class Parser
{
public:
    Parser();
    virtual ~Parser();

    static bool parseSimpleObj(string filepath, Vertex*& vertices, Triangle*& triangles, GLuint& vertex_num, GLuint& triangle_num);
    static bool parseSkinnedObj(string filepath, Vertex*& vertices, Triangle*& triangles, GLuint& vertex_num, GLuint& triangle_num, Skeleton*& skeleton);


    // static void parseObjFile(string filename, Vertex*& vertices, Triangle*& triangles, GLuint& vertex_num, GLuint& triangle_num, Skeleton* skeleton);
    // static void parseObj(string filename, Vertex*& vertices, Triangle*& triangles, GLuint& vertex_num, GLuint& triangle_num, Skeleton* skeleton); // works
    // static void parseActor(string filename, vector<string>& materials, vector<string>& meshes, mat4& matrix, Skeleton* skeleton);
    // static void parseMaterial(string filename, vec4& ambient, vec4& diffuse, vec4& specular, float& shininess, vec4& emission, GLuint& tbo_id, int& hasTexture);
    // static void parseStatic(string filename, vector<string>& materials, vector<string>& meshes, mat4& matrix);

    //  sf::Image loadTexture(std::string loadpath);

protected:
private:
};

#endif // PARSER_H
