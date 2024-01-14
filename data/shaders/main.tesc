#version 460 core

layout (vertices=3) out;

in vec3 vPosition[];
in vec2 vTexcoord[];
in vec3 vNormal[];
in vec3 vTangent[];

out vec3 tcPosition[];
out vec2 tcTexcoord[];
out vec3 tcNormal[];
out vec3 tcTangent[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
    tcTexcoord[gl_InvocationID] = vTexcoord[gl_InvocationID];
    tcNormal[gl_InvocationID] = vNormal[gl_InvocationID];
    tcTangent[gl_InvocationID] = vTangent[gl_InvocationID];

    if (gl_InvocationID == 0)
    {
        const int tessLevel = 50;

        // 3 outer and 1 inner for a triangle
        // 4 outer and 2 inner for a quad
        gl_TessLevelOuter[0] = tessLevel;
        gl_TessLevelOuter[1] = tessLevel;
        gl_TessLevelOuter[2] = tessLevel;

        gl_TessLevelInner[0] = tessLevel;
    }
}
