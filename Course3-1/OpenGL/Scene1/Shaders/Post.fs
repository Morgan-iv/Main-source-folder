#version 410 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int needInverse;

void main()
{
    if (needInverse == 0)
        FragColor = vec4(vec3(texture(screenTexture, TexCoords)), 1.0);
    else
        FragColor = vec4(1.0 - vec3(texture(screenTexture, TexCoords)), 1.0);
}
