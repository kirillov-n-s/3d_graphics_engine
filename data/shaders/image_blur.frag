#version 460 core

in vec2 vTexcoord;

out vec4 FragColor;

uniform sampler2D uImage;

vec3 gaussianBlur3(sampler2D image, vec2 point);

void main()
{
    vec3 blurredColor = gaussianBlur3(uImage, vTexcoord);
    FragColor = vec4(blurredColor, 1.0);
}

vec3 gaussianBlur3(sampler2D image, vec2 point)
{
    vec2 imageHeightWidth = vec2(textureSize(image, 0));
    float imageHeight = imageHeightWidth.x;
    float imageWidth = imageHeightWidth.y;

    vec2 du = vec2(1.0 / imageWidth, 0.0);
    vec2 dv = vec2(0.0, 1.0 / imageHeight);

    vec3 uNvN = texture(image, point - du - dv).rgb * 1.0;
    vec3 uNv0 = texture(image, point - du     ).rgb * 2.0;
    vec3 uNvP = texture(image, point - du + dv).rgb * 1.0;

    vec3 u0vN = texture(image, point      - dv).rgb * 2.0;
    vec3 u0v0 = texture(image, point          ).rgb * 4.0;
    vec3 u0vP = texture(image, point      + dv).rgb * 2.0;

    vec3 uPvN = texture(image, point + du - dv).rgb * 1.0;
    vec3 uPv0 = texture(image, point + du     ).rgb * 2.0;
    vec3 uPvP = texture(image, point + du + dv).rgb * 1.0;

    return (uNvN + uNv0 + uNvP + u0vN + u0v0 + u0vP + uPvN + uPv0 + uPvP) / 16.0;
}
