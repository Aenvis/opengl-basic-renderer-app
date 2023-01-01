#version 330 core
out vec4 FragColor;

struct Material
{
    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
    float shininess;
};

struct Light
{
    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
};

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

uniform Material material;
uniform Light lightSource;

void main()
{
    vec3 ambient = material.ambient * lightSource.ambient;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);

    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = (diff * material.diffuse ) * lightSource.diffuse;

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = (spec * material.specular) * lightSource.specular;

    vec3 resultColor = ambient + diffuse + specular;
    FragColor = vec4(resultColor, 1.0);
}