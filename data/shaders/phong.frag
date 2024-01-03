#version 460 core

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;

out vec4 FragColor;

uniform vec3 uCameraDir;
uniform sampler2D uTexture;

void main()
{
    FragColor = texture(uTexture, Texcoord);

    if (Normal != vec3(0.0))
    {
        vec3 neglightdir = -normalize(vec3(0.0, 0.0, -1.0));
        vec3 normal = normalize(Normal);

        vec3 k_ambient = vec3(0.3);
        vec3 k_diffuse = vec3(0.5);
        vec3 k_specular = vec3(1.0);
        float shininess = 32.0;

        float dotdiffuse = max(dot(normal, neglightdir), 0.0);
        vec3 reflected = normalize(reflect(neglightdir, normal));
        float dotspecular = max(dot(uCameraDir, reflected), 0.0);
        float powspecular = pow(dotspecular, shininess);

        FragColor *= vec4(k_ambient + dotdiffuse * k_diffuse + powspecular * k_specular, 1.0);
    }
}
