#version 430 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec4 inColor;

layout (location = 0) uniform mat4 projection;
layout (location = 1) uniform mat4 view;

out vec3 normal;
out vec4 color;

void main() {
    gl_Position = projection * view * vec4(inPosition, 1.0);
    normal = inNormal;
    color = inColor;
}