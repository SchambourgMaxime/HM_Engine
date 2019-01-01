#version 150 core

in vec3 in_Vertex;
in vec2 in_Pos;
in vec2 in_TexCoord0;

out vec2 coordTexture;

void main()
{
	vec2 pos = vec2(in_Vertex.x + in_Pos.x, in_Vertex.y + in_Pos.y);

	gl_Position = vec4(pos, 0.0, 1.0);

	coordTexture = in_TexCoord0;
}