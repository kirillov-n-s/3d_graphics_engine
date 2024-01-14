#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;

out vec2 vTexcoord;
out vec3 vTangent;
out vec3 vBitangent;
out vec3 vNormal;

void main()
{
    vec3 N = normalize(inNormal);
    vec3 T = normalize(inTangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    vTexcoord = inTexcoord;
    vTangent = T;
    vBitangent = B;
    vNormal = N;

    gl_Position = vec4(inTexcoord * 2.0 - 1.0, 0.0, 1.0);
}
