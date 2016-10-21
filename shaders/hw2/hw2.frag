#version 330

out vec4 fragColor;

uniform float inputTime;

void main() 
{
    vec4 finalColor = vec4(abs(sin(inputTime)),1.0,0.5,1.0);
    // Insert your code for "Slightly-More Advanced Shaders" here.

    fragColor = finalColor;
}
