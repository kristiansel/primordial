#version 330 core

// Input vertex data, different for all executions of this shader.
// layout(location = 0) in vec3 vertexPosition_modelspace;

// This is what I would have done.. is this depricated?
attribute vec4 InVertex ;
attribute vec4 bone_index ;
attribute vec4 bone_weight ;

// Values that stay constant for the whole mesh.
uniform mat4 depthMVP;

const int MAX_BONES = 100;
uniform mat4[MAX_BONES] bone_mat;

void main(){

    vec4 myvertex =  (bone_weight[0]*bone_mat[int(bone_index[0])]
                +bone_weight[1]*bone_mat[int(bone_index[1])]
                +bone_weight[2]*bone_mat[int(bone_index[2])]
                +bone_weight[3]*bone_mat[int(bone_index[3])]) * InVertex;

	gl_Position =  depthMVP * myvertex;
}

