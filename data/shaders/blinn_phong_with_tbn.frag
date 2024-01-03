#version 460 core

in vec3 vPosition;
in vec2 vTexcoord;
in mat3 vTBN;

out vec4 FragColor;

uniform vec3 uCameraDir;

uniform sampler2D uAlbedo;
uniform sampler2D uNormalMap;
uniform sampler2D uRoughness;

void main()
{
    FragColor = texture(uAlbedo, vTexcoord);

    vec3 normal = texture(uNormalMap, vTexcoord).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(vTBN * normal);

    if (normal == vec3(0.0))
        return;

    float roughness = texture(uRoughness, vTexcoord).r;

    vec3 viewDir = -normalize(uCameraDir);
    vec3 lightDir = -normalize(vec3(0.0, 0.0, -1.0));
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 materialAmbient = vec3(0.3);
    vec3 materialDiffuse = vec3(0.5);
    vec3 materialSpecular = vec3(1.0);
    float shininess = 32.0;

    float diffuse = max(dot(normal, lightDir), 0.0);
    float specular = max(dot(normal, halfwayDir), 0.0);
    specular = pow(specular, shininess);

    FragColor *= vec4(materialAmbient + diffuse * materialDiffuse + specular * materialSpecular, 1.0);
}
