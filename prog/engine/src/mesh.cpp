#include "mesh.h"

Mesh::Mesh() : vertex_num(0), triangle_num(0)
              ,vertices(nullptr), triangles(nullptr)
{
    //ctor
//    filepath = "";
//
//    vertex_num = 0;
//    triangle_num = 0;
//
//    vertices = NULL;
//    triangles = NULL;

    vbo_id  = 99999;
    ibo_id  = 99999;

    load_stage = -1;

    /// Change this at a later stage
    // set the materials <-- change this
    material.ambient = vec4(0.2, 0.2, 0.1, 1.0);
    material.diffuse = vec4(0.4, 0.8, 0.4, 1.0);
    material.specular = vec4(0.7, 0.8, 1.0, 1.0);
    material.shininess = 2;
    material.emission = vec4(0.0, 0.0, 0.0, 1.0);

    // texture?
    material.hasTexture = 0; // <--- CHANGE THIS
}

Mesh::~Mesh()
{
//    cout << "WARNING: DELETING MESH\n";
    /// release RAM pointers
    delete [] vertices;
    delete [] triangles;

    /// release video RAM buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0); // do you really need this?
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//    cout << "Deleting Buffers: " << vbo_id << " & " << ibo_id << "\n";
    glDeleteBuffers(1, &vbo_id); // is this really sufficient
    glDeleteBuffers(1, &ibo_id);

}

Mesh::Mesh(string filepath)
{
    fromFile(filepath);
}

void Mesh::fromFile(string filepath)
{
    filepath = "assets_raw/models/"+filepath+".obj";
//    this->filepath = filepath;
//    cout << filepath << "\n";

    bool res = Parser::parseSimpleObj(filepath, vertices, triangles, vertex_num, triangle_num); /// calls "new" on pointers
    if (!res) cout << "unable to load mesh " << filepath << "\n";

    load_stage = 1;

    /// send directly to graphics card
    glGenBuffers(1, &vbo_id); //must come after glewinit
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertex_num*sizeof(Vertex), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind buffer

    glGenBuffers(1, &ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangle_num*(sizeof(Triangle)), triangles, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//    cout << "Triangle number at load-time: " << triangle_num << "\n";
//    cout << "Vertex number at load-time: " << vertex_num << "\n";
//    cout << "VBO id at load-time " << vbo_id << "\n";
//    cout << "IBO id at load-time: " << ibo_id << "\n";
    load_stage = 2;

}

//void Mesh::setStageLevel(int stage)
//{
//    ///complete after rendering complete
////    if (stage > load_stage)
////    {
////        switch load_stage
////        {
////        case -1:
////        case  0:
////        case  1:
////        case  2:
////        }
////    }
////    else if (stage > load_stage)
////    {
////
////    }
////    else /// we are done
////    {
////
////    }
//
//}
Mesh::Material Mesh::getMaterial()
{
    return material;
}

GLuint Mesh::getVBOid()
{
    return vbo_id;
}

GLuint Mesh::getIBOid()
{
    return ibo_id;
}

unsigned int Mesh::getTriNum()
{
    return triangle_num;
}
