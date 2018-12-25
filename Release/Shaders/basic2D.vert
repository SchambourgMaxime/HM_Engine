#version 150 core

in vec3 in_Vertex;

uniform mat4 modelviewProjection;

void main()
{
	gl_Position = modelviewProjection * vec4(in_Vertex, 1.0);
}