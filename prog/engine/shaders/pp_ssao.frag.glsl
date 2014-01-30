# version 120


uniform sampler2D fbo_texture;
uniform sampler2D rbo_DepthTexture;
uniform float[25] x_offsets;
uniform float[25] y_offsets;

varying vec2 f_texcoord;

float rand(vec2 co)  {
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * (43758.5453+f_texcoord.x + f_texcoord.y));
}

float lin_norm_depth(vec2 uv) {
	float n = 0.1; // camera z near
    float f = 99.0; // camera z far
    float z = texture2D(rbo_DepthTexture, uv).x;
    float val = (2.0 * n) / (f + n - z * (f - n));
	return val ;
}


void main(void) {
		float thisdepth = lin_norm_depth(f_texcoord) + texture2D(fbo_texture, f_texcoord).x/texture2D(fbo_texture, f_texcoord).x-1.0;
		float occlusion = 0;
		for (int i = 0; i<25; i++) {
			float w = rand(vec2(0.0, 2.0));
			float difference = lin_norm_depth(f_texcoord+vec2(x_offsets[i], y_offsets[i]))-thisdepth;
			difference = w*difference;
			if (difference<-0.000001) {
				occlusion+=0.09/*abs(difference)*/;
			}// else if (difference >0.000001) {
			//	occlusion-=0.09;
			//}
    	}
		//occlusion = occlusion*1.0/423.0;
		occlusion = occlusion/10.0;


		//gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0)-vec4(occlusion, occlusion, occlusion, 1.0);
		gl_FragColor = vec4(occlusion, occlusion, occlusion, 1.0); // final output
}
