#version 330

layout(location = 0) in vec4 vertexPosition;
uniform float inputTime;

void main()
{
    vec4 modifiedVertexPosition = vertexPosition;
    
    modifiedVertexPosition.y = modifiedVertexPosition.y - sin(inputTime);
    
    // Insert your code for "Slightly-More Advanced Shaders" here.
    
    gl_Position = modifiedVertexPosition;
}
