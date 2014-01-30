# version 120


uniform sampler2D fbo_texture;
uniform sampler2D rbo_DepthTexture;
uniform float[25] x_offsets;
uniform float[25] y_offsets;

varying vec2 f_texcoord;

void main(void) {
		vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
		color.x = texture2D(fbo_texture, f_texcoord).x/texture2D(fbo_texture, f_texcoord).x;
		gl_FragColor = color;
}
