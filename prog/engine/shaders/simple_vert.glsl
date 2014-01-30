# version 120

varying vec3 mynormal ;
varying vec4 myvertex ;
varying vec2 mytexco ;


attribute vec3 InNormal ;
attribute vec4 InVertex ;
attribute vec2 InTexCoord ;
attribute vec3 bone_index ;
attribute vec3 bone_weight ;

uniform mat4 mv_mat;

void main() {
    //color = normalize(vec4(InColor.r, InColor.g, InColor.b, 1.0)) ;
    mynormal = InNormal ;
	mytexco = InTexCoord ;
    myvertex = InVertex ;

    gl_Position = gl_ProjectionMatrix * mv_mat * myvertex ;
}

