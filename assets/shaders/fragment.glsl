#version 330 core

struct Light
{
    vec3 colour;
    vec3 position;
};
uniform Light uLight;
uniform vec3 uCameraPos;
out vec4 FragColor;

in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;

uniform sampler2D baseColourTexture;

void main()
{
    vec3 norm = normalize(vNormal);

    //Diffuse Calc
    vec3 lightDir = normalize(uLight.position - vFragPos);
    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = diff * uLight.colour;

    //Specular Calc
    vec3 viewDir = normalize(uCameraPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    float specularStrength = 0.5;
    vec3 specular = specularStrength * spec * uLight.colour;

    //Ambient Calc
    const float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * uLight.colour;

    vec4 texColor = texture(baseColourTexture, vUV);
    vec3 result = (diffuse + specular + ambient) * texColor.xyz;
    FragColor = vec4(result, 1.0);
}