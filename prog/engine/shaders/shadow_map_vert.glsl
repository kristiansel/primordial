#version 330 core


layout(location = 0) in vec4 InVertex;
//layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec2 InTexCoord;
layout(location = 3) in vec4 bone_index;
layout(location = 4) in vec4 bone_weight;

out vec2 texcoord;

// Values that stay constant for the whole mesh.
uniform mat4 depthMVP;

// Bones
const int MAX_BONES = 100;
uniform mat4[MAX_BONES] bone_mat;

void main(){

    texcoord = InTexCoord ;

    vec4 myvertex =  (bone_weight[0]*bone_mat[int(bone_index[0])]
                +bone_weight[1]*bone_mat[int(bone_index[1])]
                +bone_weight[2]*bone_mat[int(bone_index[2])]
                +bone_weight[3]*bone_mat[int(bone_index[3])]) * InVertex;

	gl_Position = depthMVP * myvertex;
}

