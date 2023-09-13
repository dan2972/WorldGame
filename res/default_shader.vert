#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoord;
layout (location = 2) in float aLightLevel;

uniform mat4 projView;

out vec3 TexCoord;
out float LightLevel;

void main()
{
    gl_Position = projView * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    LightLevel = aLightLevel;
}    