#version 330 core

in vec3 vertexPosition;

out vec4 color;

uniform samplerCube uSkyBox;

void main()
{
    color = texture(uSkyBox, vertexPosition);
}