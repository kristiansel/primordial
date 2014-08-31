# version 330 core

varying vec2 mytexco ;

varying vec4 shadowvertex ;
varying vec3 mypos ; // camera space
varying vec3 normal ;  // camera space

varying vec4 world_pos ; // world space


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

uniform mat4 proj_mat;

// instanced variables
const int MAX_INSTANCED = 50;
uniform mat4[MAX_INSTANCED] mv_mat;
uniform mat4[MAX_INSTANCED] shadowmap_mvp_mat;
uniform mat4[MAX_INSTANCED] to_world_space_mat;

const int MAX_BONES = 100;
uniform mat4[MAX_BONES] bone_mat;

void main() {
    int instanceID = gl_InstanceID;

	mytexco = InTexCoord ;

    vec4 myvertex =   (bone_weight[0]*bone_mat[int(bone_index[0])]
                      +bone_weight[1]*bone_mat[int(bone_index[1])]
                      +bone_weight[2]*bone_mat[int(bone_index[2])]
                      +bone_weight[3]*bone_mat[int(bone_index[3])]) * InVertex;

    vec4 mynormal4 = (bone_weight[0]*bone_mat[int(bone_index[0])]
                     +bone_weight[1]*bone_mat[int(bone_index[1])]
                     +bone_weight[2]*bone_mat[int(bone_index[2])]
                     +bone_weight[3]*bone_mat[int(bone_index[3])])*vec4(InNormal.x, InNormal.y, InNormal.z, 0.0);

    vec3 mynormal = mynormal4.xyz;

    shadowvertex = shadowmap_mvp_mat[instanceID] * myvertex;
    mypos = (mv_mat[0] * myvertex).xyz ;
    normal = normalize( (mv_mat[0] * vec4(mynormal, 0.0) ).xyz) ;

    world_pos = to_world_space_mat[instanceID] * myvertex ;

    gl_Position = proj_mat * mv_mat[instanceID] * myvertex ;
}

