#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoords;

out vec2 vTexCoords;

void main()
{
    gl_Position = position;
    vTexCoords = texCoords;
}

#shader pixel
#version 330 core

uniform sampler2D uTexture;

in vec2 vTexCoords;

out vec4 color;

void main()
{
    color = texture(uTexture, vTexCoords);
}