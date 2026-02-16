#version 330 core

// Vetex information.
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

// Instance information.
layout(location = 3) in vec4 modelMatrixCol1;
layout(location = 4) in vec4 modelMatrixCol2;
layout(location = 5) in vec4 modelMatrixCol3;
layout(location = 6) in vec4 modelMatrixCol4;

out vec2 textureUv;
out vec3 fragNormal;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main()
{
    mat4 modelMatrix = mat4(modelMatrixCol1, modelMatrixCol2, modelMatrixCol3, modelMatrixCol4);

    gl_Position = uProjectionMatrix * uViewMatrix * modelMatrix  * vec4(position, 1.0);
	gl_Position.x = -gl_Position.x;

    textureUv = uv;
	
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	fragNormal = normalize(normalMatrix * normal);
	fragNormal = normalize(normal);
};