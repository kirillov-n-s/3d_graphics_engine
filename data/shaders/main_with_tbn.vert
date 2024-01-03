#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;

out vec3 vPosition;
out vec2 vTexcoord;
out mat3 vTBN;

uniform mat4 uModel;
uniform mat4 uNormalModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vec4 position = vec4(inPosition, 1.0);

    gl_Position = uProjection * uView * uModel * position;

    vPosition = vec3(uModel * position);
    vTexcoord = inTexcoord;

    vec3 N = normalize(mat3(uNormalModel) * inNormal);
    vec3 T = normalize(mat3(uNormalModel) * inTangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    vTBN = mat3(T, B, N);
}
