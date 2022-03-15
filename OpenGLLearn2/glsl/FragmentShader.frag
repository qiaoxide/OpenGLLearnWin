#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D otherTexture;

void main()
{
	FragColor=mix(texture(ourTexture,TexCoord),texture(otherTexture,TexCoord),0.2);
}