#include "parser.h"

Parser::Parser()
{
    //ctor
}

Parser::~Parser()
{
    //dtor
}

bool Parser::parseSimpleObj(string filepath, Vertex*& vertices, Triangle*& triangles, GLuint& vertex_num, GLuint& triangle_num)
{
    vector<vec3> temp_verts;
    vector<vec3> temp_norms;
    vector<vec2> temp_texcos;
    vector<Triangle> temp_tris;
    vector<Triangle> temp_texco_inds;
    vector<Triangle> temp_norm_inds;
    // vector<string> temp_b_names;
    // vector<vec3> temp_b_weights;
    string str, ret = "", cmd;
    ifstream in ;
    in.open(filepath.c_str()) ;
    if (!in.is_open())
    {
        return false;
    }
    else
    {
        getline (in, str) ;
        while (in)
        {
            if (!((str.substr(0,1)=="#") || (str=="")))
            {
                //cout << str << "\n";
                cmd = strtok(&str[0], " ");

                if (cmd=="v")
                {
                    vec3 vert;
                    vert.x = atof(strtok(NULL, " "));
                    vert.y = atof(strtok(NULL, " "));
                    vert.z = atof(strtok(NULL, " "));
                    temp_verts.push_back(vert);
                }
                if (cmd=="vn")
                {
                    vec3 norm;
                    norm.x = atof(strtok(NULL, " "));
                    norm.y = atof(strtok(NULL, " "));
                    norm.z = atof(strtok(NULL, " "));
                    temp_norms.push_back(norm);
                }
                if (cmd=="vt")
                {
                    vec2 vt;
                    vt[0] = atof(strtok(NULL, " "));
                    vt[1] = 1-atof(strtok(NULL, " "));
                    temp_texcos.push_back(vt);
                }
                if (cmd=="f")
                {
                    Triangle tri;
                    Triangle texco_tri;
                    Triangle norm_tri;

                    tri.indices[0] = atoi(strtok(NULL, " /"))-1;
                    if (temp_texcos.size()>0) texco_tri.indices[0] = atoi(strtok(NULL, " /"))-1;
                    else texco_tri.indices[0] = 0.0;
                    norm_tri.indices[0] = atoi(strtok(NULL, " /"))-1;

                    tri.indices[1] = atoi(strtok(NULL, " /"))-1;
                    if (temp_texcos.size()>0) texco_tri.indices[1] = atoi(strtok(NULL, " /"))-1;
                    else texco_tri.indices[0] = 0.0;
                    norm_tri.indices[1] = atoi(strtok(NULL, " /"))-1;

                    tri.indices[2] = atoi(strtok(NULL, " /"))-1;
                    if (temp_texcos.size()>0) texco_tri.indices[2] = atoi(strtok(NULL, " /"))-1;
                    else texco_tri.indices[0] = 0.0;
                    norm_tri.indices[2] = atoi(strtok(NULL, " /"))-1;

                    temp_tris.push_back(tri);
                    temp_texco_inds.push_back(texco_tri);
                    temp_norm_inds.push_back(norm_tri);

                }
//                if (cmd=="w")
//                {
//                    char* name = strtok(NULL, " :()");
//                    string bone_names = "";
//                    int i = 0;
//                    vec3 w = vec3(0, 0, 0);
//                    while (name != NULL && i<3)
//                    {
//                        string name_str = name;
//                        bone_names +=name_str+" ";
//                        w[i] = atof(strtok(NULL, " :()"));
//                        name = strtok (NULL, " :()");
//                        i++;
//                    }
//                    w = glm::normalize(w); //! UNSURE ABOUT THIS
//
//                    temp_b_names.push_back(bone_names);
//                    temp_b_weights.push_back(w);
//                }
            }
            getline (in, str) ;
        } // while there are more lines
        triangle_num = temp_tris.size();
        vertex_num = temp_verts.size();

        vertices = new Vertex [vertex_num]; // this function does NOT own the pointers, but fills them
        triangles = new Triangle [triangle_num]; // this function does NOT own the pointers, but fills them

        for (unsigned int i = 0; i<triangle_num; i++)
        {
            for (int k = 0; k<3; k++)
            {
                int vert_ind = temp_tris[i].indices[k];
                int texco_ind = temp_texco_inds[i].indices[k];
                int norm_ind = temp_norm_inds[i].indices[k];
//
                triangles[i].indices[k] = vert_ind;

                vertices[vert_ind].position = vec4(temp_verts[vert_ind].x, temp_verts[vert_ind].y, temp_verts[vert_ind].z, 1.0);

                if (temp_texcos.size()>0)
                {
                    vertices[vert_ind].tex_coords[0] = temp_texcos[texco_ind][0];
                    vertices[vert_ind].tex_coords[1] = temp_texcos[texco_ind][1];
                }
                else
                {
                    vertices[vert_ind].tex_coords[0] = 0.0;
                    vertices[vert_ind].tex_coords[1] = 0.0;
                }

                vertices[vert_ind].normal = temp_norms[norm_ind];

                for (unsigned int yy = 0; yy<MAX_BONE_INFLUENCES; yy++ )
                {
                    vertices[vert_ind].bone_indices[yy] = 0;
                    vertices[vert_ind].bone_weights[yy] = 0;
                }
                vertices[vert_ind].bone_weights[0] = 1.0;
            }
        }
        return true;
    }
}
//
//// Called from mesh.cpp
//void Parser::parseObjFile(string filename, Vertex*& vertices, Triangle*& triangles, GLuint& vertex_num, GLuint& triangle_num, Skeleton* skeleton)
//{
//    vector<vec3> temp_verts;
//    vector<vec3> temp_norms;
//    vector<vec2> temp_texcos;
//    vector<Triangle> temp_tris;
//    vector<Triangle> temp_texco_inds;
//    vector<Triangle> temp_norm_inds;
//    vector<string> temp_b_names;
//    vector<vec3> temp_b_weights;
//    string str, ret = "", cmd;
//    ifstream in ;
//    in.open(filename.c_str()) ;
//    if (in.is_open())
//    {
//        getline (in, str) ;
//        while (in)
//        {
//            if (!((str.substr(0,1)=="#") || (str=="")))
//            {
//                //cout << str << "\n";
//                cmd = strtok(&str[0], " ");
//
//                if (cmd=="v")
//                {
//                    vec3 vert;
//                    vert.x = atof(strtok(NULL, " "));
//                    vert.y = atof(strtok(NULL, " "));
//                    vert.z = atof(strtok(NULL, " "));
//                    temp_verts.push_back(vert);
//                }
//                if (cmd=="vn")
//                {
//                    vec3 norm;
//                    norm.x = atof(strtok(NULL, " "));
//                    norm.y = atof(strtok(NULL, " "));
//                    norm.z = atof(strtok(NULL, " "));
//                    temp_norms.push_back(norm);
//                }
//                if (cmd=="vt")
//                {
//                    vec2 vt;
//                    vt[0] = atof(strtok(NULL, " "));
//                    vt[1] = 1-atof(strtok(NULL, " "));
//                    temp_texcos.push_back(vt);
//                }
//                if (cmd=="f")
//                {
//                    Triangle tri;
//                    Triangle texco_tri;
//                    Triangle norm_tri;
//
//                    tri.indices[0] = atoi(strtok(NULL, " /"))-1;
//                    if (temp_texcos.size()>0) texco_tri.indices[0] = atoi(strtok(NULL, " /"))-1; else texco_tri.indices[0] = 0.0;
//                    norm_tri.indices[0] = atoi(strtok(NULL, " /"))-1;
//
//                    tri.indices[1] = atoi(strtok(NULL, " /"))-1;
//                    if (temp_texcos.size()>0) texco_tri.indices[1] = atoi(strtok(NULL, " /"))-1; else texco_tri.indices[0] = 0.0;
//                    norm_tri.indices[1] = atoi(strtok(NULL, " /"))-1;
//
//                    tri.indices[2] = atoi(strtok(NULL, " /"))-1;
//                    if (temp_texcos.size()>0) texco_tri.indices[2] = atoi(strtok(NULL, " /"))-1; else texco_tri.indices[0] = 0.0;
//                    norm_tri.indices[2] = atoi(strtok(NULL, " /"))-1;
//
//                    temp_tris.push_back(tri);
//                    temp_texco_inds.push_back(texco_tri);
//                    temp_norm_inds.push_back(norm_tri);
//
//                }
//                if (cmd=="w")
//                {
//                    char* name = strtok(NULL, " :()");
//                    string bone_names = "";
//                    int i = 0;
//                    vec3 w = vec3(0, 0, 0);
//                    while (name != NULL && i<3)
//                    {
//                        string name_str = name;
//                        bone_names +=name_str+" ";
//                        w[i] = atof(strtok(NULL, " :()"));
//                        name = strtok (NULL, " :()");
//                        i++;
//                    }
//                    w = glm::normalize(w); //! UNSURE ABOUT THIS
//
//                    temp_b_names.push_back(bone_names);
//                    temp_b_weights.push_back(w);
//                }
//            }
//            getline (in, str) ;
//        } // while there are more lines
//        triangle_num = temp_tris.size();
//        vertex_num = temp_verts.size();
//
//        vertices = new Vertex [vertex_num];
//        triangles = new Triangle [triangle_num];
//
//        for (int i = 0; i<triangle_num; i++)
//        {
//            for (int k = 0; k<3; k++)
//            {
//                int vert_ind = temp_tris[i].indices[k];
//                int texco_ind = temp_texco_inds[i].indices[k];
//                int norm_ind = temp_norm_inds[i].indices[k];
////
//                triangles[i].indices[k] = vert_ind;
//
//                vertices[vert_ind].position = vec4(temp_verts[vert_ind].x, temp_verts[vert_ind].y, temp_verts[vert_ind].z, 1.0);
////                if (filename == "resources/models/quad_big_tex.obj")
////                {
////                    cout<<"tri("<<i<<"/"<<triangle_num-1<<"):"<<k<<","<<vert_ind<<"/"<<vertex_num-1<<", "<<texco_ind<<"/"<<temp_texcos.size()-1<<", "<<norm_ind<<"/"<<temp_norms.size()-1<<"\n";
////                    cout<<"temp_texcos[texco_ind] = "<<temp_texcos[texco_ind][0]<<", "<<temp_texcos[texco_ind][1]<<"\n";
////                }
//                if (temp_texcos.size()>0) {
//                    vertices[vert_ind].tex_coords[0] = temp_texcos[texco_ind][0];
//                    vertices[vert_ind].tex_coords[1] = temp_texcos[texco_ind][1];
//                } else
//                {
//                    vertices[vert_ind].tex_coords[0] = 0.0;
//                    vertices[vert_ind].tex_coords[1] = 0.0;
//                }
//
//                vertices[vert_ind].normal = temp_norms[norm_ind];
//
//                for (int yy = 0; yy<MAX_BONE_INFLUENCES; yy++ )
//                {
//                    vertices[vert_ind].bone_indices[yy] = 0;
//                    vertices[vert_ind].bone_weights[yy] = 0;
//                }
//                vertices[vert_ind].bone_weights[0] = 1.0;
//
//            }
//        }
//        if (temp_b_names.size()>0)
//        {
//            for (int h = 0; h<vertex_num; h++)
//            {
//                char* name = strtok(&(temp_b_names[h])[0], " ");
//                for (int i = 0; i<3; i++)
//                {
//                    if (name != NULL)
//                    {
//                        string name_str = name;'
//                        int index_candidate = -1; // means no bone
//
//                        for (int j = 0; j<skeleton->numBones; j++)
//                        {
//                            if (skeleton->bones_rest[j].name==name_str)
//                            {
//                                index_candidate = j;
//                            }
//                        }
//                        vertices[h].bone_indices[i] = index_candidate;
//                        if (index_candidate!=-1)
//                        {
//                            vertices[h].bone_weights[i] = temp_b_weights[h][i];
//                        }
//                        else
//                        {
//                            vertices[h].bone_weights[i] = 0;
//                        }
//                    }
//                    name = strtok(NULL, " ");
//                }
//                float sum_weights = vertices[h].bone_weights[0] + vertices[h].bone_weights[1] + vertices[h].bone_weights[2];
//                if (sum_weights>0.0001)
//                {
//                    vertices[h].bone_weights[0] = vertices[h].bone_weights[0]/sum_weights;
//                    vertices[h].bone_weights[1] = vertices[h].bone_weights[1]/sum_weights;
//                    vertices[h].bone_weights[2] = vertices[h].bone_weights[2]/sum_weights;
//                }
//                else
//                {
//                    cout<<"wierd vertex weights "<<h<<"\n";
//                    vertices[h].bone_weights[0] = 1.0;
//                    vertices[h].bone_weights[1] = 0.0;
//                    vertices[h].bone_weights[2] = 0.0;
//                }
//            } // for vertices
//        } // if skeleton
//    } // if open
////    if (filename == "resources/models/quad_big_tex.obj")
////    {
////        for (int y = 0; y<vertex_num; y++)
////        {
////            int ab;
////            cout<<"vertex "<<y<<": \n";
////            cout<<"    pos: "<<vertices[y].position.x<<"\t"<<vertices[y].position.y<<"\t"<<vertices[y].position.z<<" \n";
////            cout<<"   norm: "<<vertices[y].normal.x<<"\t"<<vertices[y].normal.y<<"\t"<<vertices[y].normal.z<<" \n";
////            cout<<"  texco: "<<vertices[y].tex_coords[0]<<"\t"<<vertices[y].tex_coords[1]<<" \n";
//////            for
//////            cout<<"    b_w: "<<y<<": \n";
//////            cout<<"  b_ind: "<<y<<": \n";
////            cin>>ab;
////        }
////    }
//
//}
//
//// Called from mesh.cpp
//void Parser::parseObj(string filename, Vertex*& vertices, Triangle*& triangles, GLuint& vertex_num, GLuint& triangle_num, Skeleton* skeleton)
//{
//    vector<vec3> temp_verts;
//    vector<vec3> temp_norms;
//    vector<string> temp_b_names;
//    vector<vec3> temp_b_weights;
//    vector<Triangle> temp_tris;
////    vector<int> texco_inds;
//    vector<vec2> texcos;
//
//    string str, ret = "", cmd;
//    ifstream in ;
//    in.open(filename.c_str()) ;
//    if (in.is_open())
//    {
//        getline (in, str) ;
//        while (in)
//        {
//            if (!((str.substr(0,1)=="#") || (str=="")))
//            {
//                //cout << str << "\n";
//                cmd = strtok(&str[0], " ");
//
//                if (cmd=="v")
//                {
//                    vec3 vert;
//                    vert.x = atof(strtok(NULL, " "));
//                    vert.y = atof(strtok(NULL, " "));
//                    vert.z = atof(strtok(NULL, " "));
//                    temp_verts.push_back(vert);
//                    //end = temp_verts.size()-1;
//                }
//                if (cmd=="n")
//                {
//                    vec3 norm;
//                    norm.x = atof(strtok(NULL, " "));
//                    norm.z = atof(strtok(NULL, " "));
//                    norm.y = -atof(strtok(NULL, " "));
//                    temp_norms.push_back(norm);
//                }
//                if (cmd=="vt")
//                {
//                    vec2 vt;
//                    vt[0] = atof(strtok(NULL, " "));
//                    vt[1] = atof(strtok(NULL, " "));
//                    texcos.push_back(vt);
//                }
//                if (cmd=="f")
//                {
//                    Triangle tri;
//
//                    tri.indices[0] = atoi(strtok(NULL, " /"))-1;
////                    texco_inds.push_back(atoi(strtok(NULL, " /"))-1);
//                    tri.indices[1] = atoi(strtok(NULL, " /"))-1;
////                    texco_inds.push_back(atoi(strtok(NULL, " /"))-1);
//                    tri.indices[2] = atoi(strtok(NULL, " /"))-1;
////                    texco_inds.push_back(atoi(strtok(NULL, " /"))-1);
//
//                    temp_tris.push_back(tri);
//
//                }
//                if (cmd=="w")
//                {
//                    char* name = strtok(NULL, " :()");
//                    string bone_names = "";
//                    int i = 0;
//                    vec3 w = vec3(0, 0, 0);
//                    while (name != NULL && i<3)
//                    {
//                        string name_str = name;
//                        bone_names +=name_str+" ";
//                        w[i] = atof(strtok(NULL, " :()"));
//                        name = strtok (NULL, " :()");
//                        i++;
//                    }
//                    w = glm::normalize(w); //! UNSURE ABOUT THIS
//
//                    temp_b_names.push_back(bone_names);
//                    temp_b_weights.push_back(w);
//                }
//            }
//            getline (in, str) ;
//        }
//
//        vertex_num = temp_verts.size();
//        triangle_num = temp_tris.size();
////        cout<<"trinum = "<< temp_tris.size() << "\n";
////        cout<<"vertNum = "<<temp_verts.size() << "\n";
//
//
//
//
////            Vertex * vertices;
//        vertices = new Vertex [vertex_num];
////            Triangle * triangles;
//        triangles = new Triangle [triangle_num];
//
////        for (int k = 0; k<triangle_num; k++)
////        {
////            vertices[temp_tris[k].indices[0]].tex_coords = texcos[texco_inds[3*k]];
////            vertices[temp_tris[k].indices[1]].tex_coords = texcos[texco_inds[3*k+1]];
////            vertices[temp_tris[k].indices[2]].tex_coords = texcos[texco_inds[3*k+2]];
////        }
//
//        for (int h = 0; h<vertex_num; h++)
//        {
//            vertices[h].position = vec4(temp_verts[h].x, temp_verts[h].y, temp_verts[h].z, 1.0);
//            vertices[h].normal = temp_norms[h];
////            vertices[h].tex_coords = // THIS IS WHERE IT ALL BREAKS DOWN. INDICES OF TEX-COORDS TO NOT MATCH INDICES OF VERTICES
//            for (int yy = 0; yy<MAX_BONE_INFLUENCES; yy++ )
//            {
//                vertices[h].bone_indices[yy] = 0;
//                vertices[h].bone_weights[yy] = 0;
//            }
//            vertices[h].bone_weights[0] = 1.0;
////             Find the bones_rest
//            //cout<<"Vertex nr. "<<h<<": ";
//            //cout<<temp_b_names[h]<<": ";
//            if (temp_b_names.size()>0)
//            {
//                char* name = strtok(&(temp_b_names[h])[0], " ");
//                for (int i = 0; i<3; i++)
//                {
//                    if (name != NULL)
//                    {
//                        string name_str = name;
//                        int index_candidate = -1; // means no bone
//
//                        for (int j = 0; j<skeleton->numBones; j++)
//                        {
//                            if (skeleton->bones_rest[j].name==name_str)
//                            {
//                                index_candidate = j;
//                            }
//                        }
//                        vertices[h].bone_indices[i] = index_candidate;
//                        if (index_candidate!=-1)
//                        {
//                            vertices[h].bone_weights[i] = temp_b_weights[h][i];
//                        }
//                        else
//                        {
//                            vertices[h].bone_weights[i] = 0;
//                        }
//                    }
//                    name = strtok(NULL, " ");
//                }
//                float sum_weights = vertices[h].bone_weights[0] + vertices[h].bone_weights[1] + vertices[h].bone_weights[2];
//                if (sum_weights>0.0001)
//                {
//                    vertices[h].bone_weights[0] = vertices[h].bone_weights[0]/sum_weights;
//                    vertices[h].bone_weights[1] = vertices[h].bone_weights[1]/sum_weights;
//                    vertices[h].bone_weights[2] = vertices[h].bone_weights[2]/sum_weights;
//                }
//                else
//                {
//                    cout<<"wierd vertex weights "<<h<<"\n";
//                    vertices[h].bone_weights[0] = 1.0;
//                    vertices[h].bone_weights[1] = 0.0;
//                    vertices[h].bone_weights[2] = 0.0;
//                }
//            }
//            //cout<<"\n";
//        }
//        for (int u = 0; u<triangle_num; u++)
//        {
//            triangles[u] = temp_tris[u];
//        }
////        for (int pp = 0; pp<vertex_num; pp++) {
////            cout<<"indices: "<<vertices[pp].bone_indices[0]<<"\t"<<vertices[pp].bone_indices[1]<<"\t"<<vertices[pp].bone_indices[2]<<"\n";
////            cout<<"weights: "<<vertices[pp].bone_weights[0]<<"\t"<<vertices[pp].bone_weights[1]<<"\t"<<vertices[pp].bone_weights[2]<<"\n";
////            cout<<"\n";
////        }
//    }
//    else
//    {
//        cout<<"unable to open file: "<<filename<<"\n";
//    }
//
////        cout<<"gets out of loop";
////    if (vertex_num>200)
////    {
////        cout<<"normal: "<<vertices[125].normal.x<<", "<<vertices[125].normal.y<<", "<<vertices[125].normal.z<<"\n";
////    }
//
//
//}
//
//// Called from actor.cpp
//void Parser::parseActor(string filename, vector<string>& materials, vector<string>& meshes, mat4& matrix, Skeleton* skeleton)
//{
//    string str, ret = "", cmd, esc = " :()";  // esc is symbols ignored by the tokenizer
//    string active_mat = "none" ;
//    ifstream in ;
//    in.open(filename.c_str()) ;
//    if (in.is_open())
//    {
//        getline (in, str) ;
//        while (in)
//        {
//            if (!((str.substr(0,1)=="#") || (str=="")))
//            {
//                //cout << str << "\n";
//                cmd = strtok(&str[0], esc.c_str());
//
//                if (cmd=="material")
//                {
//                    active_mat = strtok(NULL, esc.c_str());
//                }
//                if (cmd=="mesh")
//                {
//                    meshes.push_back(strtok(NULL, esc.c_str()));
//                    materials.push_back(active_mat); // if active_mat == "", then default material
//                }
//                if (cmd=="matrix")
//                {
//                    string nl;
//                    matrix[0][0] = atof(strtok(NULL, esc.c_str()));
//                    matrix[0][1] = atof(strtok(NULL, esc.c_str()));
//                    matrix[0][2] = atof(strtok(NULL, esc.c_str()));
//                    matrix[0][3] = 0;
//                    getline(in, nl);
//                    matrix[1][0] = atof(strtok(&nl[0], esc.c_str()));
//                    matrix[1][1] = atof(strtok(NULL, esc.c_str()));
//                    matrix[1][2] = atof(strtok(NULL, esc.c_str()));
//                    matrix[1][3] = 0;
//                    getline(in, nl);
//                    matrix[2][0] = atof(strtok(&nl[0], esc.c_str()));
//                    matrix[2][1] = atof(strtok(NULL, esc.c_str()));
//                    matrix[2][2] = atof(strtok(NULL, esc.c_str()));
//                    matrix[2][3] = 0;
//                    matrix[3][0] = 0;
//                    matrix[3][1] = 0;
//                    matrix[3][2] = 0;
//                    matrix[3][3] = 1;
//                }
//                if (cmd=="skeleton")
//                {
//                    string next = strtok(NULL, esc.c_str());
//                    if (next != "none")
//                    {
//                        skeleton->loadFromBNS(next);
//                    } else
//                    {
//                        skeleton = NULL;
//                        cout<<"setting skeleton to point NULL\n";
//                    }
//                }
//                if (cmd=="animation")
//                {
//                    string name = strtok(NULL, " :()");
//                    int start = atoi(strtok(NULL, " :()"));
//                    int end =  atoi(strtok(NULL, " :()"));
//                    skeleton->addAnim(Animation(name, start, end));
//                }
//            }
//            getline (in, str) ;
//        }
//    }
//    else
//    {
//        cout << "couldn't find file: " << filename << "\n";
//    }
//}
//
//void Parser::parseMaterial(string filename, vec4& ambient, vec4& diffuse, vec4& specular, float& shininess, vec4& emission, GLuint& tbo_id, int& hasTexture)
//{
//    string str, ret = "", cmd, esc = " :()";  // esc is symbols ignored by the tokenizer
//    string texture_path = "none" ;
//    ifstream in ;
//    in.open(filename.c_str()) ;
//    if (in.is_open())
//    {
//        getline (in, str) ;
//        while (in)
//        {
//            if (!((str.substr(0,1)=="#") || (str=="")))
//            {
//                //cout << str << "\n";
//                cmd = strtok(&str[0], esc.c_str());
//
//                if (cmd=="texture")
//                {
//                    texture_path = strtok(NULL, esc.c_str());
//                }
//                if (cmd=="ambient")
//                {
//                    ambient[0] = atof(strtok(NULL, esc.c_str()));
//                    ambient[1] = atof(strtok(NULL, esc.c_str()));
//                    ambient[2] = atof(strtok(NULL, esc.c_str()));
//                    ambient[3] = atof(strtok(NULL, esc.c_str()));
//                }
//                if (cmd=="diffuse")
//                {
//                    diffuse[0] = atof(strtok(NULL, esc.c_str()));
//                    diffuse[1] = atof(strtok(NULL, esc.c_str()));
//                    diffuse[2] = atof(strtok(NULL, esc.c_str()));
//                    diffuse[3] = atof(strtok(NULL, esc.c_str()));
//                }
//                if (cmd=="specular")
//                {
//                    specular[0] = atof(strtok(NULL, esc.c_str()));
//                    specular[1] = atof(strtok(NULL, esc.c_str()));
//                    specular[2] = atof(strtok(NULL, esc.c_str()));
//                    specular[3] = atof(strtok(NULL, esc.c_str()));
//                }
//                if (cmd=="shininess")
//                {
//                    shininess = atof(strtok(NULL, esc.c_str()));
//                }
//                if (cmd=="emission")
//                {
//                    emission[0] = atof(strtok(NULL, esc.c_str()));
//                    emission[1] = atof(strtok(NULL, esc.c_str()));
//                    emission[2] = atof(strtok(NULL, esc.c_str()));
//                    emission[3] = atof(strtok(NULL, esc.c_str()));
//                }
//            }
//            getline (in, str) ;
//        }
//    }
//    else
//    {
//        cout << "couldn't find file: " << filename << "\n";
//    }
//
//    if (texture_path != "none")
//    {
////        std::cout << "loading texture " << texture_path << " ... ";
//
//        sf::Image texture;
//        if (!(texture.loadFromFile(texture_path)))
//        {
//            std::cout << "unable to load\n";
//        }
//        else
//        {
////            std::cout << "loaded!\n";
//        }
//
//        glGenTextures(1, &tbo_id);
////        cout<<"texture id: "<<tbo_id<<"\n";
//
//        //glActiveTexture( GL_TEXTURE0 );
//        glBindTexture(GL_TEXTURE_2D, tbo_id);
//        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//        sf::Vector2u size = texture.getSize();
////        std::cout<<"texture size = "<<size.x<<", "<<size.y<<"\n";
//        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, size.x, size.y,
//                          GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
//
//        glBindTexture(GL_TEXTURE_2D, 0);
//
//    GLenum errCode;
//    const GLubyte *errString;
//
//    if ((errCode = glGetError()) != GL_NO_ERROR) {
//        errString = gluErrorString(errCode);
//       fprintf (stderr, "OpenGL Error: %s\n", errString);
//    }
//
//
//        hasTexture = 1;
//    }
//    else
//    {
//        hasTexture = 0;
//    }
//
//}
//
//// called from static.cpp
//void Parser::parseStatic(string filename, vector<string>& materials, vector<string>& meshes, mat4& matrix)
//{
////    vector<string> materials;
////    vector<string> meshes;
//    string active_material;
//
//    string str, ret = "", cmd, esc = " :()";  // esc is symbols ignored by the tokenizer
//
//
//    cout<<"loading static "<<filename<<"...\n";
//    ifstream in ;
//    in.open(filename.c_str()) ;
//    if (in.is_open())
//    {
//        getline (in, str) ;
//        while (in)
//        {
//            if (!((str.substr(0,1)=="#") || (str=="")))
//            {
//                //cout << str << "\n";
//                cmd = strtok(&str[0], esc.c_str());
//
//                if (cmd=="material")
//                {
//                    active_material = strtok(NULL, esc.c_str());
//                }
//                if (cmd=="mesh")
//                {
//                    if (active_material != "")
//                    {
//                        materials.push_back(active_material);
//                    } else
//                    {
//                        materials.push_back("none");
//                    }
////                    cout<<"adding mesh\n";
//                    meshes.push_back(strtok(NULL, esc.c_str()));
//                }
//                if (cmd=="matrix")
//                {
////                    cout<<"adding matrix\n";
//                    string nl;
//                    matrix[0][0] = atof(strtok(NULL, esc.c_str()));
//                    matrix[0][1] = atof(strtok(NULL, esc.c_str()));
//                    matrix[0][2] = atof(strtok(NULL, esc.c_str()));
//                    matrix[0][3] = 0;
//                    getline(in, nl);
//                    matrix[1][0] = atof(strtok(&nl[0], esc.c_str()));
//                    matrix[1][1] = atof(strtok(NULL, esc.c_str()));
//                    matrix[1][2] = atof(strtok(NULL, esc.c_str()));
//                    matrix[1][3] = 0;
//                    getline(in, nl);
//                    matrix[2][0] = atof(strtok(&nl[0], esc.c_str()));
//                    matrix[2][1] = atof(strtok(NULL, esc.c_str()));
//                    matrix[2][2] = atof(strtok(NULL, esc.c_str()));
//                    matrix[2][3] = 0;
//                    matrix[3][0] = 0;
//                    matrix[3][1] = 0;
//                    matrix[3][2] = 0;
//                    matrix[3][3] = 1;
//                }
//            }
//            getline (in, str) ;
//        }
//    }
//    else
//    {
//        cout << "couldn't find file: " << filename << "\n";
//    }
//}

