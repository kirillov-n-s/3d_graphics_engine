#version 460 core
layout (location = 0) in vec3 inPosition;

out vec3 vPosition;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    vPosition = inPosition;
    gl_Position =  uProjection * uView * vec4(vPosition, 1.0);
}
