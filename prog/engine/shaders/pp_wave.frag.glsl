# version 120


uniform sampler2D fbo_texture;
uniform sampler2D rbo_DepthTexture;
uniform float[25] x_offsets;
uniform float[25] y_offsets;

float focal_plane = 0.5;

varying vec2 f_texcoord;

const float[25] blur = {2,   7,   12,    7,   2,
								 7,  31,   52,   31,   7,
								 15,  52,  127,   52,  15,
								 7,  31,   52,   31,   7,
								  2,   7,   12,    7,   2};

const float[25] sharpen = {-1,   -1,   -1,    -1,   -1,
								 -1,  -1,   -1,   -1,   -1,
								 -1,  -1,  25,   -1,  -1,
								 -1,  -1,   -1,   -1,   -1,
								  -1,   -1,   -1,    -1,   -1};

float lin_norm_depth(vec2 uv) {
	float n = 0.1; // camera z near
    float f = 99.0; // camera z far
    float z = texture2D(rbo_DepthTexture, uv).x;
    float val = (2.0 * n) / (f + n - z * (f - n));
	return val ;
}

float rand(vec2 co)  {
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * (43758.5453+f_texcoord.x + f_texcoord.y));
}

void main(void) {
	if (f_texcoord.x<0.5 && f_texcoord.y<0.5) {
	vec4 color = vec4(0, 0, 0, 0);
/*
	float sigma = 10*texture2D(rbo_DepthTexture, f_texcoord).x;

	float[25] gauss;

    for (int x = -2; x<3; x++) {
    	for (int y = -2; y<3; y++) {
			gauss[(y+2)+(x+2)*5] = 1.0/(2.0*3.14*sigma*sigma)*exp(-(x*x + y*y)/(2*sigma*sigma));
		}
	}
	float sum = 0;
	for (int j = 0; j<25; j++) {
		sum = sum + gauss[j];
	}
	for (int j = 0; j<25; j++) {
		gauss[j] = gauss[j]/sum;
	}




    for (int i = 0; i<25; i++) {
		color+=gauss[i]*texture2D(fbo_texture, f_texcoord+vec2(x_offsets[i], y_offsets[i]));
    }

	*/
    for (int i = 0; i<25; i++) {
		color+=sharpen[i]*texture2D(fbo_texture, f_texcoord+vec2(x_offsets[i], y_offsets[i]));
    }
	//color = color*1.0/423.0;

	mat4 col_corr = mat4(1, 0, 0, 0,
				  		 0, 1, 0, 0,
				  		 0, 0, 1, 0,
			      		 0, 0, 0, 1);

	color = col_corr*color;

	gl_FragColor = texture2D(fbo_texture, f_texcoord);
	} else if (f_texcoord.x>0.5 && f_texcoord.y<0.5) {
		//float sample_depth_val = texture2D(rbo_DepthTexture, vec2(0.49, 0.7)).x ;
		//float val = texture2D(rbo_DepthTexture, f_texcoord).x ;
		//val = val/sample_depth_val ;

		float n = 0.1; // camera z near
        float f = 99.0; // camera z far
        float z = texture2D(rbo_DepthTexture, f_texcoord).x;
        float val = (2.0 * n) / (f + n - z * (f - n));

		//val = val;

		gl_FragColor = vec4(val, val, val, 1.0);
	} else if (f_texcoord.x>0.5 && f_texcoord.y>0.5) {
		//float sample_depth_val = texture2D(rbo_DepthTexture, vec2(0.49, 0.7)).x ;
		//float val = texture2D(rbo_DepthTexture, f_texcoord).x ;
		//val = val/sample_depth_val ;

		float n = 0.1; // camera z near
        float f = 99.0; // camera z far
        float z = texture2D(rbo_DepthTexture, f_texcoord).x;
        float val = (2.0 * n) / (f + n - z * (f - n));

		//val = val;
		float foc_pos_norm = (n+focal_plane)/(f-n) ;

		if (abs(foc_pos_norm-val)>0.15) {
			vec4 color = vec4(0, 0, 0, 0);
			for (int i = 0; i<25; i++) {
				color+=blur[i]*texture2D(fbo_texture, f_texcoord+vec2(x_offsets[i], y_offsets[i]));
    		}
			color = color*1.0/423.0;
			gl_FragColor = color;
		} else {
			gl_FragColor = texture2D(fbo_texture, f_texcoord) ;
		}

	} else if (f_texcoord.x<0.5 && f_texcoord.y>0.5) {
		float thisdepth = lin_norm_depth(f_texcoord) ;
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
		occlusion = occlusion/15.0;


		gl_FragColor = texture2D(fbo_texture, f_texcoord)-vec4(occlusion, occlusion, occlusion, 1.0);
    }
	/*vec4 depth = texture2D(fbo_texture, f_texcoord);


	gl_FragColor = vec4(depth.x, depth.y, depth.z, 1.0);*/
}
