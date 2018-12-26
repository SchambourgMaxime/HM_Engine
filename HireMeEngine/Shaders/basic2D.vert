#version 150 core

in vec3 in_Vertex;

uniform mat4 modelview;
uniform mat4 projection;

void main()
{
	gl_Position = modelview * projection * vec4(in_Vertex, 1.0);
}