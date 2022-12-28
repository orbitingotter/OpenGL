#shader vertex
#version 330 core

layout (location = 0) in vec4 position;

void main()
{
    gl_Position = position;
}

#shader pixel
#version 330 core

out vec4 color;

void main()
{
    color = vec4(0.21, 0.83, 0.4, 1.0);
}