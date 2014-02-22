#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <memory>

#include "geometry.h"
#include "parser.h"
#include "object3d.h"
#include "memstreamer.hpp"

using std::string;
using std::cout;
using std::shared_ptr;

/// Should consider a getDrawInfo function that returns some sort of struct
/// at the same time checking if it is video loaded, if not video load
/// (assume context)

/// In the same way when loading from file. Could make check for valid context
/// before calling glBufferBlabla check out:
/// static bool sf::Context::IsContextActive()

/// Object3d, consider making getter/setter pos and dir


class Mesh : public Object3d /// 3d coordinates local in Prop/Actor coordinate system
{
public:
    /// Public subclass
    struct Material /// This shouldn't belong to mesh, mesh should only be geometry
    {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;
        glm::vec4 emission;

        // texture?
        bool hasTexture; // <--- CHANGE THIS
    };
public:
    /// Constructor/Destructor
    Mesh();
    explicit Mesh(string filepath);
    virtual ~Mesh();

    /// Public member functions
    void fromFile(string mesh_key);
    void fromFile2(string mesh_key);
    // void setStageLevel(int stage);
    Material getMaterial();             // called by shader
    GLuint getVBOid();                  // called by shader
    GLuint getIBOid();                  // called by shader
    unsigned int getTriNum();           // called by shader



protected:
private:
   // string filepath;

    unsigned int vertex_num;
    unsigned int triangle_num;

    Vertex* vertices;
    Triangle* triangles;

    GLuint vbo_id;
    GLuint ibo_id;

    /// load_stage = 0, 1, or 2;
    /// -1: "No file on record                       use procedural default geometry
    ///  0: "Not loaded (file on disk)"              use filename
    ///  1: "loaded to RAM (file on RAM)"            use vertex/tri pointer
    ///  2: "loaded to video (file on video card)"   use vbo_id/ibo_id

    /// Current number in use by graphics card;



    /// To be separated out at a later stage
    // set the materials <-- change this
    Material material;


};

#endif // MESH_H
