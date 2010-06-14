uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform ivec4 layers;

varying vec2 coord;

void main(void)
{
	vec4 color[4];
	vec4 color_final = vec4(0.0);
	
	if (layers.x == 1)
		color_final += texture2D(tex0, coord);
	if (layers.y == 1)
		color_final += texture2D(tex1, coord);
	if (layers.z == 1)
		color_final += texture2D(tex2, coord);
	if (layers.w == 1)
		color_final += texture2D(tex3, coord);

	gl_FragColor = color_final;
}
