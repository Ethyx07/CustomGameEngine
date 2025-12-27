#version 330 core

struct Light
{
    vec3 colour;
    vec3 position;
};
uniform Light uLight;
out vec4 FragColor;

in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;

uniform sampler2D baseColourTexture;

void main()
{
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLight.position - vFragPos);
    float diff = max(dot(norm, lightDir), 0);

    vec3 diffuse = diff * uLight.colour;

    vec4 texColor = texture(baseColourTexture, vUV);
    FragColor = texColor * vec4(diffuse, 1.0);
}