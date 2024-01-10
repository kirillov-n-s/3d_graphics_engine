#version 460 core

in vec3 vPosition;

out vec4 FragColor;

uniform samplerCube uEnvironment;
uniform float uRoughness;

#define PI 3.14159265359

float RadicalInverse_VdC(uint bits);
vec2 HammersleySequenceSample(uint i, uint N);
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);
float NDF_TrowbridgeReitzGGX(float NdotH, float roughness);

void main()
{
    vec3 N = normalize(vPosition);
    vec3 R = N;
    vec3 V = R;

    vec3 prefilteredColor = vec3(0.0);

    const uint nSamples = 4096u;
    float totalWeight = 0.0;
    for (uint sampleInd = 0u; sampleInd < nSamples; ++sampleInd)
    {
        vec2 Xi = HammersleySequenceSample(sampleInd, nSamples);
        vec3 H  = ImportanceSampleGGX(Xi, N, uRoughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotH = max(dot(N, H), 0.0);
        float HdotV = max(dot(V, H), 0.0);

        float D = NDF_TrowbridgeReitzGGX(NdotH, uRoughness);
        float pdf = (D * NdotH / (4.0 * HdotV)) + 0.0001;

        float resolution = 4096.0; // resolution of source cubemap (per face), bug: needs to be a uniform
//        float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
//        float saSample = 1.0 / (float(nSamples) * pdf + 0.0001);
//        float mipLevel = uRoughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

        // rewrite using logarithm rules for theoretically better accuracy
        float logPdf = log2(pdf);
        float logNsamples = log2(nSamples);
        float logResolution = log2(resolution);
        const float C1 = log2(4.0 * PI);
        const float C2 = log2(6.0);
        float mipLevelByRoughness = -logNsamples - logPdf - C1 + C2 + 2 * logResolution;
        float mipLevel = uRoughness == 0.0 ? 0.0 : 0.5 * mipLevelByRoughness;

        vec3 envColor = textureLod(uEnvironment, L, mipLevel).rgb;

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

float NDF_TrowbridgeReitzGGX(float NdotH, float roughness)
{
    float aSqr = pow(roughness, 4.0);
    float denom = NdotH * NdotH * (aSqr - 1.0) + 1.0;
    return aSqr / (PI * denom * denom);
}
