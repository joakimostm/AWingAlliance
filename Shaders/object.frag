#version 450

in vec3 x_normal;
in vec3 x_color;
smooth in vec2 x_texcoord;

uniform sampler2D tex;
uniform bool bUseColor;

out vec4 out_Color;


void main(void)
{
	vec4 mytex = texture(tex, x_texcoord);
	if(bUseColor)
	{
		mytex = vec4(0.7, 0.0, 0.0, 0.0);
	}
	out_Color = mytex;
}