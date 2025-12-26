#version 330 core
out vec4 FragColor;

in vec2 vUV;

uniform sampler2D baseColourTexture;

void main()
{
    vec4 texColor = texture(baseColourTexture, vUV);
    FragColor = texColor;
}