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


uniform vec3 uCameraPos;
uniform vec3 uDirectionalLight;

out VS_OUT
{
    vec3 vNormal;
    vec2 vTexCoords;
    vec4 vFragPosLight;

    vec3 vWorldPosTS;
    vec3 vCameraTS;
    vec3 vLightTS;
}vsOut;

void main()
{
    gl_Position = (uProj * uView * uModel) * aPosition;

    mat3 modelVector = transpose(inverse(mat3(uModel)));

    //vec3 T = normalize(modelVector * aTangent);
    //vec3 B = normalize(modelVector * aBitangent);
    //vec3 N = normalize(modelVector * aNormal);

    vec3 T = normalize(vec3(uModel * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(uModel * vec4(aNormal, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    //B = normalize(modelVector * aBitangent);


    mat3 TBN = transpose(mat3(T, B, N)); // inverse of TBN - transform from world space to local tangent space

    vsOut.vWorldPosTS = TBN * vec3(uModel * aPosition);
    vsOut.vCameraTS = TBN * uCameraPos;
    vsOut.vLightTS = TBN * uDirectionalLight;

    vsOut.vNormal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
    vsOut.vTexCoords = aTexCoords;
    vsOut.vFragPosLight = uSMProj * uModel * aPosition;
}

#shader pixel
#version 450 core

in VS_OUT
{
    vec3 vNormal;
    vec2 vTexCoords;
    vec4 vFragPosLight;

    vec3 vWorldPosTS;
    vec3 vCameraTS;
    vec3 vLightTS;
}fsIn;



layout (binding = 0) uniform sampler2D uTextureDiffuse0;
layout (binding = 1) uniform sampler2D uTextureSpecular0;
layout (binding = 2) uniform sampler2D uTextureNormal0;
layout (binding = 3) uniform sampler2D uTextureDisplacement0;


// shadow map
layout (binding = 4) uniform sampler2D uSMTexture;

out vec4 fragColor;

uniform vec3 uDirectionalColor;

// test variables
uniform bool uPcfEnabled;
uniform bool uShadowEnabled;
uniform int uSampleRange;

uniform bool uNormalMappingEnabled;
uniform bool uParallaxMappingEnabled;

vec3 DirLightCalc(vec3 normalTS, vec2 newTexCoords);
float ShadowCalc(vec4 fragPosLight);
vec2 ParallaxCalc(vec2 oldTexCoords, vec3 viewDir);

void main()
{
    // parallax mapping
    vec2 newTexCoords = fsIn.vTexCoords;
    if(uParallaxMappingEnabled)
    {
        newTexCoords = ParallaxCalc(fsIn.vTexCoords, normalize(fsIn.vCameraTS - fsIn.vWorldPosTS));
    }

    // normal mapping
    vec3 normalTS = vec3(0, 0, 1.0);    // default normal in tangent space
    if(uNormalMappingEnabled)
    {
        normalTS = texture(uTextureNormal0, newTexCoords).rgb;
        normalTS = (normalTS * 2.0) - 1.0;
    }

    fragColor = vec4(DirLightCalc(normalTS, newTexCoords), 1.0);
}

// Parallax Calc - Parallax Mapping - returns new tex coords
vec2 ParallaxCalc(vec2 texCoords, vec3 viewDir)
{
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy /viewDir.z * 0.005;
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(uTextureDisplacement0, currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(uTextureDisplacement0, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    // parallax occulsion
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(uTextureDisplacement0, prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}


// Light Calc - Phong Model - returns color of fragment
vec3 DirLightCalc(vec3 normalTS, vec2 newTexCoords)
{
    // Constants
    float diffuseStrength = 0.7f;
    float specularStrength = 0.7f;
    float specularPower  = 32;
    float a =  0.01f, b = 0.045f, c = 1.0f;

    float diffuse, specular, ambient;

    // directional light
    vec3 lightDir = normalize(-fsIn.vLightTS);
    vec3 reflectDir = reflect(-lightDir, normalTS);
    vec3 cameraDir = normalize(fsIn.vCameraTS - fsIn.vWorldPosTS);

    diffuse = diffuseStrength * max(dot(normalize(normalTS), lightDir), 0.0);
    specular = specularStrength * pow(max(dot(reflectDir, cameraDir), 0.0), specularPower);
    ambient = 0.4f;

    float shadow = 0.0f;
    if(uShadowEnabled)
        shadow = ShadowCalc(fsIn.vFragPosLight);

    vec3 ambientColor =  ambient * texture(uTextureDiffuse0, newTexCoords).rgb;// * uDirectionalColor;
    vec3 diffuseColor =  diffuse * texture(uTextureDiffuse0, newTexCoords).rgb * uDirectionalColor;
    vec3 specularColor = specular * texture(uTextureSpecular0, newTexCoords).rgb * uDirectionalColor;

    vec3 finalColor = ((diffuseColor + specularColor) * (1.0 - shadow)  + ambientColor);
    return finalColor;
}

// Shadow Calc - Shadow Mapping - returns value between 0.0 to 1.0 denoting amount of shadow
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