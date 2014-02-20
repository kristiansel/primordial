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

    /// Change this at a later stage
    // set the materials <-- change this
    material.ambient = glm::vec4(0.2, 0.2, 0.1, 1.0);
    material.diffuse = glm::vec4(0.4, 0.8, 0.4, 1.0);
    material.specular = glm::vec4(0.7, 0.8, 1.0, 1.0);
    material.shininess = 2;
    material.emission = glm::vec4(0.0, 0.0, 0.0, 1.0);

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

void Mesh::fromFile(string mesh_key)
{
    string filepath = "assets_raw/models/"+mesh_key+".obj";
//    this->filepath = filepath;
//    cout << filepath << "\n";

    //Skeleton* skeleton;
    //skeleton->loadFromBNS(filepath); /// The wierdest and unsafest code ever...

    bool res = Parser::parseSimpleObj(filepath, vertices, triangles, vertex_num, triangle_num); /// calls "new" on pointers

    //std::weak_ptr<Skeleton> skel_wptr = ResourceManager[mesh_key] ; /// No access to resource manager, nor should it
    /// bool res = Parser::parseSkinnedObj(filepath, vertices, triangles, vertex_num, triangle_num, skeleton); /// calls "new" on pointers
    if (!res) std::cerr << "unable to load mesh " << filepath << "\n";

//    load_stage = 1;

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
//    load_stage = 2;
///    delete skeleton;
    if (mesh_key == "anim_test")
    {
        std::cout << "loading from bgeo:" << mesh_key << std::endl;
        fromFile2();
    }

}

void Mesh::fromFile2()
{

    bool debug = true;

    string filepath = "assets_bin/models/anim_test.bgeo";

    std::streampos filesize;
    char * memblock;

    std::ifstream myFile (filepath, std::ios::in | std::ios::binary | std::ios::ate);

    if (myFile.is_open())
    {
        filesize = myFile.tellg();
        memblock = new char [filesize];
        myFile.seekg (0, std::ios::beg);
        myFile.read (memblock, filesize);
        myFile.close();

        if (debug) std::cout << "filesize: "<< filesize << " bytes" << "\n";

        std::string filetype;
        filetype.resize(4*sizeof(char));      /// 4 bytes     file type
        int verMaj;                           /// 4 bytes     version major
        int verMin;                           /// 4 bytes     version minor

        MemStreamer reader(memblock, 0, filesize);
        reader.chomp(&filetype[0],  4*sizeof(char));
        reader.chomp(&verMaj,       1*sizeof(int));
        reader.chomp(&verMin,       1*sizeof(int));

        if (debug) std::cout << "filetype: " << filetype << "\n";
        if (debug) std::cout << "version: " << verMaj << "." << verMin << "\n";

        reader.chomp(&vertex_num,   1*sizeof(int));

        if (debug) std::cout << "num vertices: " << vertex_num << "\n";

        delete[] memblock;

    }
    else
    {
        std::cerr << "unable to open file " << filepath << std::endl;
    }
}

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
