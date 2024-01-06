#version 460 core

in vec3 vPosition;

out vec4 FragColor;

uniform samplerCube uEnvironment;
uniform float uRoughness;

#define PI 3.14159265359

float RadicalInverse_VdC(uint bits);
vec2 HammersleySequenceSample(uint i, uint N);
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);

void main()
{
    vec3 N = normalize(vPosition);
    vec3 R = N;
    vec3 V = R;

    float exposure = 2.0;

    const uint nSamples = 4096u;
    float totalWeight = 0.0;
    vec3 prefilteredColor = vec3(0.0);
    for (uint sampleInd = 0u; sampleInd < nSamples; ++sampleInd)
    {
        vec2 Xi = HammersleySequenceSample(sampleInd, nSamples);
        vec3 H  = ImportanceSampleGGX(Xi, N, uRoughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        vec3 envColor = texture(uEnvironment, L).rgb;
        envColor = vec3(1.0) - exp(-envColor * exposure);
//        envColor = pow(envColor, vec3(1.0 / 2.2));

        float NdotL = max(dot(N, L), 0.0);
        prefilteredColor += envColor * NdotL;
        totalWeight += NdotL;
    }
    prefilteredColor = prefilteredColor / totalWeight;

    FragColor = vec4(prefilteredColor, 1.0);
}

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 HammersleySequenceSample(uint i, uint N)
{
    return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;

    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // from tangent-space vector to world-space sample vector
    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}
