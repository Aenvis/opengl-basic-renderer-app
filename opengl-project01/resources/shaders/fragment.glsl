#version 460 core
out vec4 fragColor;

in vec3 ourColor;
in vec2 texCoords;

uniform sampler2D objTexture1;
uniform sampler2D objTexture2;
uniform float mixValue;

void main()
{
	fragColor = mix(texture(objTexture1, texCoords), texture(objTexture2, vec2(texCoords.x, texCoords.y)), mixValue);
};