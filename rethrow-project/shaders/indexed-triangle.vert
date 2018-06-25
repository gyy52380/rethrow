//VERTEX SHADER
//draws indexed textured triangle, takes in Vertex

#version 330 core

uniform mat4 transform;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 color;

out VS_OUT_FS_IN
{
	vec2 uv;
	vec4 color;

} vs_out;


void main()
{
	gl_Position = transform*vec4(position, 0, 1);

	vs_out.uv = vec2(uv.x, /*1.0f-*/uv.y);
	vs_out.color = color;
}

