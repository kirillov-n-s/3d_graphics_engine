#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord;
layout (location = 2) in vec3 inNormal;

out vec3 Position;
out vec3 Normal;
out vec2 Texcoord;

uniform mat4 uModel;
uniform mat4 uNormalModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vec4 position = vec4(inPosition, 1.0);

    gl_Position = uProjection * uView * uModel * position;

    Position = vec3(uModel * position);
    Normal = mat3(uNormalModel) * inNormal;
    Texcoord = inTexcoord;
}
