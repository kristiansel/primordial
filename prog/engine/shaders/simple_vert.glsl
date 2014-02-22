# version 120

varying vec3 mynormal ;
varying vec4 myvertex ;
varying vec2 mytexco ;


attribute vec3 InNormal ;
attribute vec4 InVertex ;
attribute vec2 InTexCoord ;
attribute vec4 bone_index ;
attribute vec4 bone_weight ;

uniform mat4 mv_mat;

const int MAX_BONES = 50;
uniform mat4[MAX_BONES] bone_mat;

void main() {
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
    gl_Position = gl_ProjectionMatrix * mv_mat * myvertex ;
}

