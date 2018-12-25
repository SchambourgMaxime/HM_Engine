#version 150 core

in vec2 coordTexture;

uniform sampler2D _texture;

out vec4 out_Color;

void main()
{
    // Couleur finale du pixel
    vec4 color = texture(_texture, coordTexture);

    if(color.a < 1)
	discard;
    else
    	out_Color = color;
}