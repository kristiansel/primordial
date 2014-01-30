# version 120


uniform sampler2D fbo_texture;
uniform sampler2D rbo_DepthTexture;
uniform float[25] x_offsets;
uniform float[25] y_offsets;

varying vec2 f_texcoord;

uniform float blur[25] = float[](2,   7,   12,    7,   2,
                                 7,  31,   52,   31,   7,
                                 15,  52,  127,   52,  15,
                                 7,  31,   52,   31,   7,
                                 2,   7,   12,    7,   2);

// uniform needed for no re-initialization for every pixel


void main(void)
{
    vec4 color = vec4(0, 0, 0, 0);
    for (int i = 0; i<25; i++)
    {
        color+=blur[i]*texture2D(fbo_texture, f_texcoord+vec2(x_offsets[i], y_offsets[i]));
    }
    color = color*1.0/423.0;

    //mat4 col_corr = mat4(1, 0.5, 2.0, 0,
    //			  		 0.3, 1, 0, 0,
    //			  		 1.2, 0, 1, 0,
    //		      		 0, 0, 0, 1);

    //color = col_corr*color;

    gl_FragColor = color;
}
