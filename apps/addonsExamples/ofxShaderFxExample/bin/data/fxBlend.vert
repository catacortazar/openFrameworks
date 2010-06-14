
varying vec2 coord;

void main(void)
{
	gl_Position = ftransform();
	coord = gl_MultiTexCoord0.st;
	//gl_FrontColor = gl_Color;
}
