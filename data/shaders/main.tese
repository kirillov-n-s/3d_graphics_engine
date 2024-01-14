#version 460 core

layout (triangles, equal_spacing, ccw) in;

in vec3 tcPosition[];
in vec2 tcTexcoord[];
in vec3 tcNormal[];
in vec3 tcTangent[];

out vec3 tePosition;
out vec2 teTexcoord;
out vec3 teNormal;
out float teHeight;
out mat3 teTBN;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform sampler2D uHeightMap;
uniform float uHeightScale;
uniform float uHeightBias;

vec3 baryInterp(vec3 a, vec3 b, vec3 c, vec3 uvw);
vec2 baryInterp(vec2 a, vec2 b, vec2 c, vec3 uvw);

float centralDiff3(sampler2D fn, vec2 point, vec2 step);
float centralDiff5(sampler2D fn, vec2 point, vec2 step);

void main()
{
    vec3 position = baryInterp(tcPosition[0], tcPosition[1], tcPosition[2], gl_TessCoord.xyz);
    vec2 texcoord = baryInterp(tcTexcoord[0], tcTexcoord[1], tcTexcoord[2], gl_TessCoord.xyz);
    vec3 normal   = baryInterp(  tcNormal[0],   tcNormal[1],   tcNormal[2], gl_TessCoord.xyz);
    vec3 tangent  = baryInterp( tcTangent[0],  tcTangent[1],  tcTangent[2], gl_TessCoord.xyz);

    float height = texture(uHeightMap, texcoord).x * uHeightScale + uHeightBias;
    position += normal * height;

    vec2 mapHeightWidth = vec2(textureSize(uHeightMap, 0));
    float mapHeight = mapHeightWidth.x;
    float mapWidth = mapHeightWidth.y;

    const float eps = 0.005;
    vec2 du = vec2(1.0 / mapWidth, 0.0) * eps;
    vec2 dv = vec2(0.0, 1.0 / mapHeight) * eps;
    float dhdu = uHeightScale * mapWidth * centralDiff3(uHeightMap, texcoord, du);
    float dhdv = uHeightScale * mapHeight * centralDiff3(uHeightMap, texcoord, dv);

    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    vec3 displacedNormal = normalize(normal + tangent * -dhdu + bitangent * -dhdv);

    tePosition = position;
    teTexcoord = texcoord;
    teNormal = displacedNormal;
    teHeight = (height - uHeightBias) / uHeightScale;
    teTBN = TBN;

    gl_Position = uProjection * uView * uModel * vec4(position, 1.0);
}

vec3 baryInterp(vec3 a, vec3 b, vec3 c, vec3 uvw)
{
    return a * uvw.x + b * uvw.y + c * uvw.z;
}

vec2 baryInterp(vec2 a, vec2 b, vec2 c, vec3 uvw)
{
    return a * uvw.x + b * uvw.y + c * uvw.z;
}

float centralDiff3(sampler2D fn, vec2 point, vec2 step)
{
    float dfPos = texture(fn, point + step).x;
    float dfNeg = texture(fn, point - step).x;
    return (dfPos - dfNeg) * 0.5;
}

float centralDiff5(sampler2D fn, vec2 point, vec2 step)
{
    float dfPos1 = texture(fn, point + 1.0 * step).x;
    float dfNeg1 = texture(fn, point - 1.0 * step).x;
    float dfPos2 = texture(fn, point + 2.0 * step).x;
    float dfNeg2 = texture(fn, point - 2.0 * step).x;
    vec4 values = vec4(dfPos1, dfNeg1, dfPos2, dfNeg2);
    vec4 coeffs = vec4(8.0, -8.0, -1.0, 1.0);
    return dot(values, coeffs) / 12.0;
}
