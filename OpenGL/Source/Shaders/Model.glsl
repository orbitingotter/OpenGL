#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 vTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    vTexCoords = aTexCoords;
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}


#shader pixel
#version 330 core

in vec2 vTexCoords;

uniform sampler2D uTextureDiffuse0;

out vec4 color;


void main()
{
    color = texture(uTextureDiffuse0, vTexCoords);
}