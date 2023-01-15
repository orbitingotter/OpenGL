#shader vertex
#version 330 core

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vNormal;
out vec4 vWorldPos;
out vec2 vTexCoords;

void main()
{
    gl_Position = (uProj * uView * uModel) * aPosition;

    vNormal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
    vWorldPos = uModel * aPosition;
    vTexCoords = aTexCoords;
}

#shader pixel
#version 330 core

in vec3 vNormal;
in vec4 vWorldPos;
in vec2 vTexCoords;

uniform vec3 uLightPos;
uniform vec3 uCameraPos;
uniform sampler2D uTextureDiffuse0;
uniform sampler2D uTextureSpecular0;
uniform sampler2D uTextureNormal0;


out vec4 fragColor;

void main()
{
    // Material Constants
    vec3 materialColor = vec3(1.0, 1.0, 1.0);
    float diffuseStrength = 1.0f;
    float specularStrength = 1.0f;
    float specularPower  = 32;
    float a =  0.0075f, b = 0.045, c = 1.0f;

    float diffuse, specular, ambient;


    vec3 lightDir = normalize(uLightPos - vWorldPos.xyz);
    vec3 cameraDir = normalize(uCameraPos - vWorldPos.xyz);
    float x = length(uLightPos - vWorldPos.xyz);
    float attentuation = 1 / (a * pow(x,2) + b * x + c);
    vec3 reflectDir = reflect(-lightDir, vNormal);

    ambient = 0.2f;
    diffuse = diffuseStrength * attentuation * max(dot(normalize(vNormal), lightDir), 0.0);
    specular = specularStrength * attentuation * pow(max(dot(reflectDir, cameraDir), 0.0), specularPower);

    vec3 ambientColor = ambient * texture(uTextureDiffuse0, vTexCoords).rgb;
    vec3 diffuseColor = diffuse * texture(uTextureDiffuse0, vTexCoords).rgb;
    vec3 specularColor = specular * texture(uTextureSpecular0, vTexCoords).rgb;

    vec3 finalColor = (diffuseColor + specularColor + ambientColor) * materialColor;
    fragColor = vec4(finalColor, 1.0);
}