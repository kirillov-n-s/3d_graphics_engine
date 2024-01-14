#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;

out vec3 vPosition;
out vec2 vTexcoord;
out vec3 vNormal;
out vec3 vTangent;

uniform mat4 uModel;
uniform mat4 uNormalModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0);

    vPosition = vec3(uModel * vec4(inPosition, 1.0));
    vTexcoord = inTexcoord;
    vNormal = normalize(mat3(uNormalModel) * inNormal);
    vTangent = normalize(mat3(uNormalModel) * inTangent);
}
