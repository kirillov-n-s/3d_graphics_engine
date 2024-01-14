#version 460 core

in vec2 vTexcoord;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vNormal;

out vec4 FragColor;

uniform sampler2D uHeightMap;
uniform float uHeightScale;

float centralDiff3(sampler2D fn, vec2 point, vec2 step);

void main()
{
    vec2 texHeightWidth = vec2(textureSize(uHeightMap, 0));
    float texHeight = texHeightWidth.x;
    float texWidth = texHeightWidth.y;

    const float eps = 0.005;
    vec2 du = vec2(1.0 / texWidth, 0.0) * eps;
    vec2 dv = vec2(0.0, 1.0 / texHeight) * eps;
    float dhdu = uHeightScale * texWidth * centralDiff3(uHeightMap, vTexcoord, du);
    float dhdv = uHeightScale * texHeight * centralDiff3(uHeightMap, vTexcoord, dv);

    mat3 TBN = mat3(vTangent, vBitangent, vNormal);
    vec3 displacedNormalInWorldSpace = normalize(vNormal + vTangent * -dhdu + vBitangent * -dhdv);
    vec3 displacedNormalInTangentSpace = inverse(TBN) * displacedNormalInWorldSpace;

    FragColor = vec4((displacedNormalInTangentSpace + 1.0) * 0.5, 1.0);
}

float centralDiff3(sampler2D fn, vec2 point, vec2 step)
{
    float dfPos = texture(fn, point + step).x;
    float dfNeg = texture(fn, point - step).x;
    return (dfPos - dfNeg) * 0.5;
}
