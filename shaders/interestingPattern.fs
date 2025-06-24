#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;
uniform float redAdd;

void main() {
    vec4 color = texture(texture0, fragTexCoord);
    color.r = mod(color.r + redAdd, 1.0);  
    fragColor = color;
}
