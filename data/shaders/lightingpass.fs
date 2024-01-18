#version 430 core

out vec3 fragColor;

in vec2 uv;

layout (binding = 0) uniform sampler2D gColor;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D gMetallicRoughness;
layout (binding = 3) uniform sampler2D gDepth;

layout (location = 0) uniform float lighting;
layout (location = 1) uniform float color;

void main() {
    vec3 normal = texture(gNormal, uv).rgb;
    vec3 lightDir = normalize(vec3(0.3, 1.3, -0.4));
    vec3 color = mix(vec3(1.0, 1.0, 1.0), texture(gColor, uv).rgb, color);

    float diff = clamp(dot(normal, lightDir), 0.0, 1.0);


    fragColor = mix(color, color * diff + color * 0.1, lighting);
}