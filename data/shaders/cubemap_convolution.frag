#version 460 core

in vec3 vPosition;

out vec4 FragColor;

uniform samplerCube uEnvironment;

#define PI 3.14159265359

void main()
{
    float exposure = 1.0;
    vec3 envColor = texture(uEnvironment, vPosition).rgb;
//    envColor = envColor / (envColor + vec3(1.0));
    envColor = vec3(1.0) - exp(-envColor * exposure);
    envColor = pow(envColor, vec3(1.0 / 2.2));
    FragColor = vec4(envColor, 1.0);
}