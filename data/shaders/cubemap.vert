#version 460 core

layout (location = 0) in vec3 inPosition;

uniform mat4 uProjection;
uniform mat4 uView;

out vec3 vPosition;

void main()
{
    vPosition = inPosition;
    mat4 rotView = mat4(mat3(uView));
    vec4 clipPos = uProjection * rotView * vec4(vPosition, 1.0);
    gl_Position = clipPos.xyww;
}