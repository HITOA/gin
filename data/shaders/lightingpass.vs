#version 430 core

out vec2 uv;

const vec2 positions[4] = vec2[](
    vec2(-1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0)
);

void main() {
    uv = (positions[gl_VertexID] + 1.0) * 0.5;
    gl_Position = vec4(positions[gl_VertexID], 1.0, 1.0);
}