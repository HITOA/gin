#version 410 core

#include "vinlighting.glsl"
#include "vinshadows.glsl"

out vec4 fragcolor;

in struct FsInput {
    vec4 positionWS; //World space position
    vec4 positionLS; //Light space position
    vec3 normalOS;
    vec3 cameraPosition;
} fsinput;

uniform float _TriplanarSharpness;

uniform sampler2D _TopDiffuseTex;
uniform sampler2D _SideDiffuseTex;

vec4 TriplanarSampling(vec4 xy, vec4 xz, vec4 yz, vec3 normalWS, float sharpness) {
    vec3 blendWeight = pow(abs(normalWS), vec3(sharpness));
    blendWeight = blendWeight / (blendWeight.x + blendWeight.y + blendWeight.z);

    return xy * blendWeight.z + xz * blendWeight.y + yz * blendWeight.x;
}

void main()
{
    float gamma = 2.2;

    vec3 topDiff = pow(texture(_TopDiffuseTex, fsinput.positionWS.xz / 4.0).xyz, vec3(gamma));
    vec3 sideXDiff = pow(texture(_SideDiffuseTex, fsinput.positionWS.xy / 4.0).xyz, vec3(gamma));
    vec3 sideZDiff = pow(texture(_SideDiffuseTex, fsinput.positionWS.zy / 4.0).xyz, vec3(gamma));

    vec4 diffuse = TriplanarSampling(vec4(sideXDiff, 1.0), vec4(topDiff, 1.0), vec4(sideZDiff, 1.0), fsinput.normalOS, _TriplanarSharpness);

    InputData inputData;
    inputData.positionWS = fsinput.positionWS.xyz;
    inputData.normalWS = fsinput.normalOS;
    inputData.viewDirectionWS = normalize(fsinput.cameraPosition.xyz - fsinput.positionWS.xyz);

    SurfaceData surfaceData;
    surfaceData.albedo = diffuse.xyz;
    surfaceData.metallic = 0.0;
    surfaceData.roughness = 0.8;
    surfaceData.alpha = diffuse.w;

    BRDFData brdfData;
    InitializeBRDFData(inputData, surfaceData, brdfData);

    vec3 color = CalculatePBR(GetMainLight(), inputData, surfaceData, brdfData);
    color += diffuse.xyz * vec3(0.015, 0.015, 0.02);
    
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / gamma)); 

    fragcolor = vec4(color, 1.0);
}