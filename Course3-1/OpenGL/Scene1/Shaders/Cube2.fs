#version 410 core
out vec4 FragColor;


struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Position;  
in vec3 Normal;  
 
 
uniform PointLight pointLights;
uniform vec3 cameraPos;
uniform samplerCube skybox;

vec3 gridSamplingDisk[20] = vec3[]
(
    vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
    vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
    vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
    vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color);

void main()
{
    vec3 norm = normalize(Normal);
	vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, norm);
	vec3 viewDir = -I; //normalize(cameraPos - Position);
    
    vec3 color = vec3(0.0, 0.0, 0.0);
    int samples = 20;
    float viewDistance = length(cameraPos - Position);
    float diskRadius = (1.0 + (viewDistance / 100.0)) / 125.0;
    for(int i = 0; i < samples; ++i)
    {
        vec3 iterColor = texture(skybox, R + gridSamplingDisk[i] * diskRadius).rgb;
        color += iterColor;
    }
    color /= samples;
    
    //vec3 color = texture(skybox, R).rgb;
	vec3 result = CalcPointLight(pointLights, norm, Position, viewDir, color); 
	FragColor = vec4(result, 1.0);
} 

// точечный источник отсвещения
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    //float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    //vec3 ambient = light.ambient * color;
    //vec3 diffuse = light.diffuse * diff * color;
    vec3 specular = light.specular * spec * color;
    //ambient *= attenuation;
    //diffuse *= attenuation;
    specular *= attenuation;
    return (color + specular);
}
