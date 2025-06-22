#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;      
uniform sampler1D redBuffer;     
uniform int maxIndex;            
uniform int imageWidth;
uniform int imageHeight;

void main() {
    vec2 uv = fragTexCoord;
    vec4 color = texture(texture0, uv);

    int x = int(uv.x * float(imageWidth));
    int y = int(uv.y * float(imageHeight));
    int index = (y * imageWidth + x) % maxIndex;

    float newRed = texelFetch(redBuffer, index, 0).r;

    fragColor = vec4(newRed, color.g, color.b, color.a);
}
