#version 460 core

in vec3 vPosition;

out vec4 FragColor;

uniform samplerCube uEnvironment;

#define PI 3.14159265359

void main()
{
    vec3 normal = normalize(vPosition);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, normal));
    up = normalize(cross(normal, right));

    float exposure = 2.0;

    vec3 irradiance = vec3(0.0);

    float sampleDelta = 0.025;
    int nSamples = 0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta, ++nSamples)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
            vec3 envColor = texture(uEnvironment, sampleVec).rgb;
            envColor = vec3(1.0) - exp(-envColor * exposure);
//            envColor = pow(envColor, vec3(1.0 / 2.2));
            irradiance += envColor * cos(theta) * sin(theta);
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nSamples));

    FragColor = vec4(irradiance, 1.0);
}