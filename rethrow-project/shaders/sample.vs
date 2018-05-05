#version 330 core

uniform float factor;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 color;

out vec4 frag_color;

void main()
{
    gl_Position = vec4(position, 0, 1);

    frag_color = color;
}