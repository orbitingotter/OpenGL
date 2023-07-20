#shader vertex
#version 450 core

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;


uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uSMProj;

out VS_OUT
{
    vec3 vNormal;
    vec4 vWorldPos;
    vec2 vTexCoords;
    vec4 vFragPosLight;
    mat3 vTBN;
}vsOut;

void main()
{
    gl_Position = (uProj * uView * uModel) * aPosition;

    mat3 modelVector = transpose(inverse(mat3(uModel)));

    vec3 T = normalize(modelVector * aTangent);
    vec3 B = normalize(modelVector * aBitangent);
    vec3 N = normalize(modelVector * aNormal);

    vsOut.vTBN = mat3(T, B, N);

    vsOut.vNormal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
    vsOut.vWorldPos = uModel * aPosition;
    vsOut.vTexCoords = aTexCoords;
    vsOut.vFragPosLight = uSMProj * uModel * aPosition;
}

#shader pixel
#version 450 core

in VS_OUT
{
    vec3 vNormal;
    vec4 vWorldPos;
    vec2 vTexCoords;
    vec4 vFragPosLight;
    mat3 vTBN;
}fsIn;


uniform vec3 uCameraPos;
uniform vec3 uDirectionalLight;
uniform vec3 uDirectionalColor;

layout (binding = 0) uniform sampler2D uTextureDiffuse0;
layout (binding = 1) uniform sampler2D uTextureSpecular0;
layout (binding = 2) uniform sampler2D uTextureNormal0;

// shadow map
layout (binding = 3) uniform sampler2D uSMTexture;

out vec4 fragColor;

// shadow test variables
uniform bool uPcfEnabled;
uniform bool uShadowEnabled;
uniform int uSampleRange;

// normal map test variable
uniform bool uNormalMappingEnabled;

vec3 DirLightCalc(vec3 normalTS);
float ShadowCalc(vec4 fragPosLight);


void main()
{
    // normal mapping
    vec3 normalTS = fsIn.vNormal;   // since varying is read-only
    if(uNormalMappingEnabled)
    {
        normalTS = texture(uTextureNormal0, fsIn.vTexCoords).rgb;
        normalTS = (normalTS * 2.0) - 1.0;
        normalTS = normalize(fsIn.vTBN * normalTS);
    }


    fragColor = vec4(DirLightCalc(normalTS), 1.0);
}


// Light Calc Functions
vec3 DirLightCalc(vec3 normalTS)
{
    // Constants
    float diffuseStrength = 0.7f;
    float specularStrength = 0.7f;
    float specularPower  = 32;
    float a =  0.01f, b = 0.045f, c = 1.0f;

    float diffuse, specular, ambient;

    // directional light
    vec3 lightDir = normalize(-uDirectionalLight);
    vec3 reflectDir = reflect(-lightDir, normalTS);
    vec3 cameraDir = normalize(uCameraPos - fsIn.vWorldPos.xyz);

    diffuse = diffuseStrength * max(dot(normalize(normalTS), lightDir), 0.0);
    specular = specularStrength * pow(max(dot(reflectDir, cameraDir), 0.0), specularPower);
    ambient = 0.4f;

    float shadow = 0.0f;
    if(uShadowEnabled)
        shadow = ShadowCalc(fsIn.vFragPosLight);

    vec3 ambientColor =  ambient * texture(uTextureDiffuse0, fsIn.vTexCoords).rgb;// * uDirectionalColor;
    vec3 diffuseColor =  diffuse * texture(uTextureDiffuse0, fsIn.vTexCoords).rgb * uDirectionalColor;
    vec3 specularColor = specular * texture(uTextureSpecular0, fsIn.vTexCoords).rgb * uDirectionalColor;

    vec3 finalColor = ((diffuseColor + specularColor) * (1.0 - shadow)  + ambientColor);
    return finalColor;
}

float ShadowCalc(vec4 fragPosLight)
{
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    projCoords = (projCoords * 0.5) + 0.5;

    float closestDepth = texture(uSMTexture, projCoords.xy).r;
    float currentDepth = projCoords.z;

    //float bias = max(0.005 * (1.0 - dot(normalize(vNormal), normalize(-uDirectionalLight))), 0.005);
    float bias = 0.00;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    // pcf filter
    if(uPcfEnabled)
    {
        vec2 texelSize = 1.0 / textureSize(uSMTexture, 0);

        const int loopRange = (uSampleRange - 1) / 2;

        for(int x = -loopRange; x <= loopRange; x++)
        {
            for(int y = -loopRange; y <= loopRange; y++)
            {
                float pcfDepth = texture(uSMTexture, projCoords.xy + vec2(x, y) * texelSize).r;
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
        shadow /= pow(uSampleRange,2);
    }

    return shadow;
}