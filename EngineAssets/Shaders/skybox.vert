#version 330 core

// Vetex information.
layout(location = 0) in vec3 position;

out vec3 vertexPosition;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main()
{
	vec4 pos = uProjectionMatrix * uViewMatrix * vec4(position, 1.0f);
	
	gl_Position = vec4(pos.xyww);
	
	vertexPosition = vec3(position.x, position.y, position.z);
}