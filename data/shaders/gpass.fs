#version 430 core

layout (location = 0) out vec3 gColor;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gMetallicRoughness;

in vec3 normal;

void main() {
    gNormal = normal;
    gColor = vec3(0.97, 0.97, 0.97);
}