# version 330 core

varying vec4 myvertex ;

attribute vec4 vertex ;

uniform mat4 mv_mat;

void main() {
    myvertex = vertex ;

    gl_Position = mv_mat * myvertex ;
}

