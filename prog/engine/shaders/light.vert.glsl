# version 120

const int MAX_BONE_INFLUENCES = 3;
const int MAX_BONES = 50;
//varying vec4 color ;
varying vec3 mynormal ;
varying vec4 myvertex ;
varying vec2 mytexco ;
//varying vec4 trvertex ;
//varying vec3 ReflectDir ;

attribute vec3 InNormal ;
attribute vec4 InVertex ;
attribute vec2 InTexCoord ;
attribute vec3 bone_index ;
attribute vec3 bone_weight ;

//uniform int cubeMapped;
uniform float time;
uniform mat4 mv_mat;

uniform mat4[MAX_BONES] bone_mat;
uniform mat4[MAX_BONES] nobo_mat;

void main() {
    //color = normalize(vec4(InColor.r, InColor.g, InColor.b, 1.0)) ;
    mynormal = InNormal ;
	mytexco = InTexCoord ;
    myvertex = InVertex ;

    if (int(bone_index[0])==-1 || int(bone_index[1])==-1 || int(bone_index[2])==-1) {
    } else {
        myvertex = (bone_weight[0]*bone_mat[int(bone_index[0])]+bone_weight[1]*bone_mat[int(bone_index[1])]+bone_weight[2]*bone_mat[int(bone_index[2])])*InVertex;

        vec4 _mynormal = (bone_weight[0]*nobo_mat[int(bone_index[0])]+
                    bone_weight[1]*nobo_mat[int(bone_index[1])]+
                    bone_weight[2]*nobo_mat[int(bone_index[2])])*vec4(InNormal.x, InNormal.y, InNormal.z, 0.0);

        mynormal = _mynormal.xyz;
    }

	    gl_Position = gl_ProjectionMatrix * mv_mat * myvertex ;
}

