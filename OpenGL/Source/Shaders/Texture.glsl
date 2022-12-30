#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoords;

uniform mat4 uMVP;
out vec2 vTexCoords;


void main()
{
    gl_Position = uMVP * position;
    vTexCoords = texCoords;
}

#shader pixel
#version 330 core

uniform sampler2D uTexture;

in vec2 vTexCoords;

out vec4 color;

void main()
{
    color = vec4(1.0);
    color = texture(uTexture, vTexCoords);
}