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

#define S 1. // Scale

void main()
{
	vec2 yUV = fsinput.positionWS.xz;
    vec2 xUV = fsinput.positionWS.zy;
    vec2 zUV = fsinput.positionWS.xy;

    vec3 blendWeights = pow(abs(fsinput.normalOS), vec3(2.0));
    blendWeights = blendWeights / (blendWeights.x + blendWeights.y + blendWeights.z);

    vec2 uv = floor(S * xUV);
	vec3 c1 = mix(vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), mod(uv.x + uv.y, 2.));
    
    uv = floor(S * yUV);
	vec3 c2 = mix(vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), mod(uv.x + uv.y, 2.));
    
    uv = floor(S * zUV);
	vec3 c3 = mix(vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), mod(uv.x + uv.y, 2.));

    vec3 c = c1 * blendWeights.x + c2 * blendWeights.y + c3 * blendWeights.z;

    InputData inputData;
    inputData.positionWS = fsinput.positionWS.xyz;
    inputData.normalWS = fsinput.normalOS;
    inputData.viewDirectionWS = normalize(fsinput.cameraPosition.xyz - fsinput.positionWS.xyz);

    SurfaceData surfaceData;
    surfaceData.albedo = vec3(1.0, 1.0, 1.0);
    //surfaceData.metallic = mr.r;
    //surfaceData.roughness = mr.g;
    //surfaceData.alpha = tex.a;

    BRDFData brdfData;
    InitializeBRDFData(inputData, surfaceData, brdfData);

    vec3 color = CalculatePBR(GetMainLight(), inputData, surfaceData, brdfData) + 0.025;
    color *= c;
    color *= mix(vec3(0.4, 0.4, 0.2), vec3(0.3, 0.9, 0.1), max(fsinput.normalOS.y, 0.0));
    
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    fragcolor = vec4(color, 1.0);
}