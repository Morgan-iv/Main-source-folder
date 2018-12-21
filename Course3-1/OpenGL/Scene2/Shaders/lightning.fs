#version 410 core

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 color;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
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

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform samplerCube depthMap;
uniform PointLight lightSource;
uniform Material material;
uniform vec3 viewPos;
uniform float far_plane;

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 fragPos);
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir, vec3 fragPos);
float ShadowCalculation(vec3 fragPos);

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // фаза 1: Направленный источник освещения
    //vec3 result = CalcDirLight(dirLight, norm, viewDir);
    vec3 result = vec3(0.0, 0.0, 0.0);
    result += CalcPointLight(lightSource, norm, viewDir, FragPos);
    // фаза 2: Точечные источники
    //for(int i = 0; i < 4; i++)
    //    result += CalcPointLight(pointLights[i], norm, viewDir, FragPos);
    // фаза 3: фонарик
    //result += CalcSpotLight(spotLight, norm, viewDir, FragPos);

    color = vec4(result, 1.0);
}


float ShadowCalculation(vec3 fragPos, PointLight light)
{
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
        
    // display closestDepth as debug (to visualize depth cubemap)
    // FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
        
    return shadow;
}


vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 fragPos)
{
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    float shadow = ShadowCalculation(fragPos, light);

    return (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    diffuse  *= intensity;
    specular *= intensity;
    return (ambient + diffuse + specular) * attenuation;
}
