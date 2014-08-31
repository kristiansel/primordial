# version 330 core

varying vec3 mynormal ; // Model space coordinates
varying vec4 myvertex ;  // Model space coordinates
varying vec2 mytexco ;

varying vec4 shadowvertex ;

//attribute vec3 InNormal ;
//attribute vec4 InVertex ;
//attribute vec2 InTexCoord ;
//attribute vec4 bone_index ;
//attribute vec4 bone_weight ;

//
layout(location = 0) in vec4 InVertex;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec2 InTexCoord;
layout(location = 3) in vec4 bone_index;
layout(location = 4) in vec4 bone_weight;



uniform mat4 mv_mat;
uniform mat4 proj_mat;
uniform mat4 shadowmap_mvp_mat;

const int MAX_BONES = 100;
uniform mat4[MAX_BONES] bone_mat;

void main() {
    int instanceID = gl_InstanceID;
    //color = normalize(vec4(InColor.r, InColor.g, InColor.b, 1.0)) ;
    mynormal = InNormal ;
	mytexco = InTexCoord ;
    myvertex = InVertex ;

    myvertex =  (bone_weight[0]*bone_mat[int(bone_index[0])]
                +bone_weight[1]*bone_mat[int(bone_index[1])]
                +bone_weight[2]*bone_mat[int(bone_index[2])]
                +bone_weight[3]*bone_mat[int(bone_index[3])]) * myvertex;

    vec4 mynormal4 = (bone_weight[0]*bone_mat[int(bone_index[0])]
                     +bone_weight[1]*bone_mat[int(bone_index[1])]
                     +bone_weight[2]*bone_mat[int(bone_index[2])]
                     +bone_weight[3]*bone_mat[int(bone_index[3])])*vec4(InNormal.x, InNormal.y, InNormal.z, 0.0);

    mynormal = mynormal4.xyz;

    shadowvertex = shadowmap_mvp_mat * myvertex;

    gl_Position = proj_mat * mv_mat * myvertex ;
}

