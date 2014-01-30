#include "geometry.h"
//
//Vertex Vertex::lerp(Vertex v1, Vertex v2)
//{
//    Vertex temp;
//
////    cout<<"    pos: "<<v1.position.x<<"\t"<<v1.position.y<<"\t"<<v1.position.z<<"\n";
////    cout<<"    pos: "<<v2.position.x<<"\t"<<v2.position.y<<"\t"<<v2.position.z<<"\n";
//
//
//
//    temp.position = 0.5*(v1.position + v2.position);
//
////    cout<<"   ipos: "<<temp.position.x<<"\t"<<temp.position.y<<"\t"<<temp.position.z<<"\n";
//
//    temp.normal = glm::normalize((v1.normal + v2.normal));
//    temp.tex_coords[0] = 0.5*(v1.tex_coords[0] + v2.tex_coords[0]);
//    temp.tex_coords[1] = 0.5*(v1.tex_coords[1] + v2.tex_coords[1]);
//    for (int i = 0; i<MAX_BONE_INFLUENCES; i++)
//    {
//        temp.bone_indices[i] = v1.bone_indices[i];
//        temp.bone_weights[i] = v1.bone_weights[i];
//    }
//
////    cout<<"    pos: "<<temp.position.x<<"\t"<<temp.position.y<<"\t"<<temp.position.z<<"\n";
//    return temp;
//
//}
//
//Vertex Vertex::lerp(vector<Vertex>& verts_in)
//{
//    Vertex temp;
//
////    cout<<"    pos: "<<v1.position.x<<"\t"<<v1.position.y<<"\t"<<v1.position.z<<"\n";
////    cout<<"    pos: "<<v2.position.x<<"\t"<<v2.position.y<<"\t"<<v2.position.z<<"\n";
//
//    float n_verts = verts_in.size();
//    float scalefactor = 1.0/n_verts;
//
//    temp.position = vec4(0.0, 0.0, 0.0, 0.0);
//    temp.normal = vec3(0.0, 0.0, 0.0);
//    temp.tex_coords[0] = 0.0;
//    temp.tex_coords[1] = 0.0;
//
//
//    for (vector<Vertex>::iterator it = verts_in.begin(); it<verts_in.end(); it++)
//    {
//        temp.position = temp.position + it->position;
//        temp.normal = temp.normal + it->normal;
//        temp.tex_coords[0] = temp.tex_coords[0] + it->tex_coords[0];
//        temp.tex_coords[1] = temp.tex_coords[1] + it->tex_coords[1];
//    }
//
//    for (int i = 0; i<MAX_BONE_INFLUENCES; i++)
//    {
//        temp.bone_indices[i] = verts_in[0].bone_indices[i];
//        temp.bone_weights[i] = verts_in[0].bone_weights[i];
//    }
//
//    temp.position = scalefactor*temp.position;
//    temp.normal = scalefactor*temp.normal;
//    temp.tex_coords[0] = scalefactor*temp.tex_coords[0];
//    temp.tex_coords[1] = scalefactor*temp.tex_coords[1];
//
//    return temp;
//
//}
