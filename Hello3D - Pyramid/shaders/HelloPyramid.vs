#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
out vec4 finalColor;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	finalColor = vec4(color, 1.0);
}