#version 330 core
out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Spotlight
{
    vec3 position;
    vec3 direction;
    float cutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in mat4 View;

uniform Material material;
uniform Spotlight spotlight;

void main()
{
    vec3 lightPos = vec3(View * vec4(spotlight.position, 1.0f));
    vec3 front = vec3(View * vec4(spotlight.direction, 1.0f));

    vec3 lightDir = normalize(lightPos - FragPos);

    float theta = dot(lightDir, normalize(-front));

    vec3 resultColor;
    if(spotlight.cutoff < theta)
    {
        vec3 ambient = texture(material.diffuse, TexCoords).rgb * spotlight.ambient;

        vec3 normal = normalize(Normal);    
        float diff = max(dot(normal, lightDir), 0.0f);
        vec3 diffuse = diff * texture(material.diffuse, TexCoords).rgb * spotlight.diffuse;

        vec3 viewDir = normalize(-FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = (spec * texture(material.specular, TexCoords).rgb) * spotlight.specular;

        float distance = length(lightPos - FragPos);
        float attenuation = spotlight.constant + spotlight.linear * distance + spotlight.quadratic * (distance * distance);

        float luminosity = 1.0f / attenuation;

        diffuse *= luminosity;
        specular *= luminosity;

        resultColor = ambient + diffuse + specular;
    }
    else resultColor = spotlight.ambient * texture(material.diffuse, TexCoords).rgb;

    FragColor = vec4(resultColor, 1.0f);
}