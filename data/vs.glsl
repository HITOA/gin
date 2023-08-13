#version 410 core

#include "vin.glsl"

VIN_POSITION vec3 _position;
VIN_NORMAL vec3 _normal;

out struct FsInput {
    vec4 positionWS; //World space position
    vec4 positionLS; //Light space position
    vec3 normalOS;
    vec3 cameraPosition;
} fsinput;

void main()
{
    fsinput.positionWS = vin_matrix_model * vec4(_position, 1.0);
    fsinput.positionLS = vin_matrix_lightspace * vec4(_position, 1.0);
    fsinput.normalOS = _normal;
    fsinput.cameraPosition = _WorldSpaceCameraPosition;

    gl_Position = ((vin_matrix_mvp) * vec4(_position, 1.0));
}