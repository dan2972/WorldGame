#version 410 core
out vec4 FragColor;  
in vec3 TexCoord;
in float LightLevel;

uniform sampler2DArray texArray;
  
void main()
{
    FragColor = texture(texArray, TexCoord)* LightLevel;
}