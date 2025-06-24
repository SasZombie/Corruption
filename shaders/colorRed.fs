#version 330 core

uniform vec4 customColor;
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

void main()
{
    finalColor = customColor;
}