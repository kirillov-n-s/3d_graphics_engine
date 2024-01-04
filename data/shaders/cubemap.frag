#version 460 core

in vec3 vPosition;

out vec4 FragColor;

uniform samplerCube uEnvironment;

void main()
{
    float exposure = 2.0;
    vec3 envColor = texture(uEnvironment, vPosition).rgb;
//    envColor = envColor / (envColor + vec3(1.0));
    envColor = vec3(1.0) - exp(-envColor * exposure);
    envColor = pow(envColor, vec3(1.0 / 2.2));
    FragColor = vec4(envColor, 1.0);
}