#version 460 core
out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D objTexture1;
uniform sampler2D objTexture2;

void main()
{
	fragColor = mix(texture(objTexture1, texCoords), texture(objTexture2, texCoords), 0.35f);
};