#shader vertex
#version 450 core

layout (location = 0) in vec4 aPosition;
layout (location = 2) in vec2 aTexCoords;

out vec2 vTexCoords;

void main()
{
    gl_Position = aPosition;
    vTexCoords = aTexCoords;
}

#shader pixel
#version 450 core

#define GAMMA 2.2

layout (binding = 0) uniform sampler2D uTexture;
uniform bool uGamma;
uniform int uToneMappingAlgLevel;
uniform float uExposure;

in vec2 vTexCoords;

out vec4 fragColor;

void main()
{
    vec3 color = texture(uTexture, vTexCoords).rgb;

    // hdr tonemapping

    if(uToneMappingAlgLevel == 0)               // Reinhard
        color = color / (color + vec3(1.0));
    else if (uToneMappingAlgLevel == 1)            // Exposure
        color = vec3(1.0) - exp(-color * uExposure);

    // gamma correction
    if(uGamma)
        color = pow(color, vec3(1.0 / GAMMA));

    fragColor = vec4(color, 1.0);
}