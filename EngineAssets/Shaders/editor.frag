#version 330 core

in vec2 textureUv;
in vec3 fragNormal;

out vec4 color;

uniform sampler2D uTexture;

void main()
{
    color = texture(uTexture, textureUv);
};