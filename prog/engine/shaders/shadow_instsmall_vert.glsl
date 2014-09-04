#version 330 core


layout(location = 0) in vec4 InVertex;
layout(location = 2) in vec2 InTexCoord;

out vec2 texcoord;

// Values that stay constant for the whole mesh.
uniform sampler1D worldpos_tex;
uniform mat4 light_mvp;

void main(){
    int instanceID = gl_InstanceID;

    texcoord = InTexCoord ;

    vec4 worldpos_sample = texelFetch(worldpos_tex, instanceID, 0);

	//vec4 worldpos_sample = vec4(instanceID*3.0, 0.0, 0.0, 1.0);
    float theta_z = worldpos_sample.a;

    mat4 world_pos_matrix = mat4(
           cos(theta_z),        0.0,            sin(theta_z),       0.0, // first column (not row!)
           0.0,                 1.0,            0.0,                0.0, // second column
           -sin(theta_z),       0.0,            cos(theta_z),       0.0,  // third column
           worldpos_sample.x, worldpos_sample.y, worldpos_sample.z, 1.0 ); // assume only translation

    mat4 depthMVP = light_mvp * world_pos_matrix;

	gl_Position = depthMVP * InVertex;
}

