#version 460 core

in vec3 vPosition;
in vec2 vTexcoord;
in mat3 vTBN;

out vec4 FragColor;

uniform vec3 uCameraPos;

uniform sampler2D uAlbedo;
uniform sampler2D uNormalMap;
uniform sampler2D uRoughness;
uniform sampler2D uMetallic;
uniform sampler2D uAO;

uniform samplerCube uIrradiance;

#define PI 3.14159265359

vec3 CookTorranceBRDF(vec3 N, vec3 V, vec3 L, vec3 c, vec3 Li, float roughness, float metallic, vec3 F0);
float NDF_TrowbridgeReitzGGX(vec3 N, vec3 H, float roughness);
float Geometry_SchlickGGX(float NdotV, float k);
float Geometry_Smith(vec3 N, vec3 V, vec3 L, float k);
vec3 Fresnel_Schlick(float cosTheta, vec3 F0);
vec3 Fresnel_Schlick_WithRoughness(float cosTheta, vec3 F0, float roughness);
float k_direct(float roughness);
float k_IBL(float roughness);

float sqr(float x);
float dotclamp0(vec3 x, vec3 y);
float clamp01(float x);

void main()
{
    vec3 lightPos = vec3(0.0, 0.0, 50.0);
    vec3 lightColor = vec3(1.0) * 10;

    vec3 albedo = texture(uAlbedo, vTexcoord).rgb;

    vec3 normal = texture(uNormalMap, vTexcoord).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(vTBN * normal);

    float roughness = texture(uRoughness, vTexcoord).r;

//    float metallic = texture(uMetallic, vTexcoord).r;
    float metallic = 0.0;
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

//    float ao = texture(uAO, vTexcoord).r;
    float ao = 1.0;

    vec3 viewDir = normalize(uCameraPos - vPosition);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < 0; ++i)  // loop over light sources
    {
        // Li - light radiance
        vec3 lightDir = normalize(lightPos - vPosition);
        float lightDistance = length(lightPos - vPosition);
        float lightAttenuation = 1.0 / (1.0 + 0.2 * lightDistance);
        vec3 radiance = lightColor * lightAttenuation;

        // BRDF
        Lo += CookTorranceBRDF(
            normal, viewDir, lightDir,
            albedo, radiance,
            roughness, metallic, F0);
    }

    // indirect lighting
    float cosTheta = dotclamp0(normal, viewDir);
    vec3 kS = Fresnel_Schlick_WithRoughness(cosTheta, F0, roughness);
    vec3 kD = 1.0 - kS;
    vec3 irradiance = texture(uIrradiance, normal).rgb;
    vec3 diffuse = irradiance * albedo;
    vec3 ambient = kD * diffuse * ao;

//    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    // tone mapping + gamma correction
    color = color / (color + vec3(1.0));
//    color = vec3(1.0) - exp(-color);
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}

vec3 CookTorranceBRDF(
    vec3 N, vec3 V, vec3 L,
    vec3 c, vec3 Li,
    float roughness, float metallic, vec3 F0)
{
    vec3 H = normalize(L + V);

    float k = k_direct(roughness);
    float D = NDF_TrowbridgeReitzGGX(N, H, roughness);
    float G = Geometry_Smith(N, V, L, k);
    float cosTheta = dotclamp0(H, V);
    vec3 F = Fresnel_Schlick(cosTheta, F0);

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

    vec3 diffuse = kD * c / vec3(PI);

    float NdotL = dotclamp0(N, L);
    float NdotV = dotclamp0(N, V);
    vec3 specular = D * F * G / (4.0 * NdotV * NdotL + 1e-4);

    vec3 Fr = diffuse + specular;
    return Fr * Li * NdotL;
}

float NDF_TrowbridgeReitzGGX(vec3 N, vec3 H, float roughness)
{
    float NdotH = dotclamp0(N, H);
    float aSqr = sqr(sqr(roughness));
    float denom = sqr(NdotH) * (aSqr - 1.0) + 1.0;
    return aSqr / (PI * sqr(denom));
}

float Geometry_SchlickGGX(float NdotV, float k)
{
    return NdotV / (NdotV * (1.0 - k) + k);
}

float Geometry_Smith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = dotclamp0(N, V);
    float NdotL = dotclamp0(N, L);
    float ggxV = Geometry_SchlickGGX(NdotV, k);
    float ggxL = Geometry_SchlickGGX(NdotL, k);
    return ggxV * ggxL;
}

vec3 Fresnel_Schlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp01(1.0 - cosTheta), 5.0);
}

vec3 Fresnel_Schlick_WithRoughness(float cosTheta, vec3 F0, float roughness)
{
    vec3 roughnessTerm = max(vec3(1.0 - roughness), F0);
    return F0 + (roughnessTerm - F0) * pow(clamp01(1.0 - cosTheta), 5.0);
}

float k_direct(float roughness)
{
    return sqr(roughness + 1.0) / 8.0;
}

float k_IBL(float roughness)
{
    return sqr(roughness) / 2.0;
}

float sqr(float x)
{
    return x * x;
}

float dotclamp0(vec3 x, vec3 y)
{
    return max(dot(x, y), 0.0);
}

float clamp01(float x)
{
    return clamp(x, 0.0, 1.0);
}
