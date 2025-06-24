#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 redShift;

void main()
{
    vec2 redCoord = fragTexCoord + redShift;
    float r = texture(texture0, redCoord).r;
    
    vec4 original = texture(texture0, fragTexCoord);

    finalColor = vec4(r, original.g, original.b, original.a);
}
