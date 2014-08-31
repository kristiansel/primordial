#version 330 core

// Input data
in vec2 texcoord;

// Ouput data
layout(location = 0) out float fragmentdepth;


uniform sampler2D tex; // the active texture


void main()
{
    if (texture(tex, texcoord.st).a < 0.80)
    {
        discard;
    }
    else
    {
        fragmentdepth = gl_FragCoord.z;
    }
}
