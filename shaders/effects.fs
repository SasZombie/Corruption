#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;
uniform float redAdd;
uniform float greenAdd;
uniform float blueAdd;

uniform float redShiftX;
uniform float redShiftY;

uniform float greenShiftX;
uniform float greenShiftY;

uniform float blueShiftX;
uniform float blueShiftY;


void main()
{
    vec4 color = texture(texture0, fragTexCoord);

    //Shift
    float redCoordX = fragTexCoord.x + redShiftX;
    float redCoordY = fragTexCoord.y + redShiftY;

    vec2 redCoord = vec2(redCoordX, redCoordY);

    float greenCoordX = fragTexCoord.x + greenShiftX;
    float greenCoordY = fragTexCoord.y + greenShiftY;

    vec2 greenCoord = vec2(greenCoordX, greenCoordY);

    float blueCoordX = fragTexCoord.x + blueShiftX;
    float blueCoordY = fragTexCoord.y + blueShiftY;

    vec2 blueCoord = vec2(blueCoordX, blueCoordY);

    //Pattern
    float addedRed = texture(texture0, redCoord).r;
    float addedGreen = texture(texture0, greenCoord).g;
    float addedBlue = texture(texture0, blueCoord).b;
    
    addedRed = mod(addedRed + redAdd, 1.0);  
    addedGreen = mod(addedGreen + greenAdd, 1.0);  
    addedBlue = mod(addedBlue + blueAdd, 1.0);  

    color.r = addedRed;
    color.g = addedGreen;
    color.b = addedBlue;

    fragColor = color;
}
