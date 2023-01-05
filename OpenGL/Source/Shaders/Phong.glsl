#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vNormal;
out vec4 vWorldPos;

void main()
{
    gl_Position = (uProj * uView * uModel) * position;

    vNormal = mat3(transpose(inverse(uModel))) * normal;
    vWorldPos = uModel * position;
}

#shader pixel
#version 330 core

in vec3 vNormal;
in vec4 vWorldPos;

uniform vec3 uLightPos;


out vec4 fragColor;

void main()
{
    // Material Constants
    vec3 materialColor = vec3(1.0, 1.0, 1.0);
    float diffuse, specular, ambient;

    vec3 lightDir = normalize(uLightPos - vWorldPos.xyz);
    float x = length(uLightPos - vWorldPos.xyz);
    float a =  0.0075f, b = 0.045, c = 1.0f;
    float attentuation = 1 / (a * pow(x,2) + b * x + c);

    ambient = 0.2f;
    diffuse = attentuation * max(dot(normalize(vNormal), lightDir), 0.0);

    vec3 finalColor = (diffuse + ambient) * materialColor;
    fragColor = vec4(finalColor, 1.0);
}