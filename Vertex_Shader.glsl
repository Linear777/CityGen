layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in mat4 offsets;

uniform mat4 MVP;
uniform float t;

out vec4 color;

void main(void)
{
	color = in_color;
	gl_Position = MVP * offsets * vec4(in_position, 1.0);
}