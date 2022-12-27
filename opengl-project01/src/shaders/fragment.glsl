#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);

    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 512);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 resultColor = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(resultColor, 1.0);
}