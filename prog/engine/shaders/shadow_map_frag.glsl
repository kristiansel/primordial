#version 330 core

// Ouput data
layout(location = 0) out float fragmentdepth;


void main(){
	fragmentdepth = gl_FragCoord.z;
	//gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
