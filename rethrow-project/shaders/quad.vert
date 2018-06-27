//VERTEX SHADER
//position -> geometry shader
//width, height -> geometry shader
//uv -> fragment shader
#version 330 core

uniform mat4 transform;
//uniform vec2 wh_scale_uniform;
#define wh_scale vec2(transform[0][0], transform[1][1]);

layout(location = 0) in vec2 position; //position of left-most quad vertex
layout(location = 1) in vec2 wh;   //quad widt and height

layout(location = 2) in vec2 tex_coord;//left-most texture coord on the atlas
layout(location = 3) in vec2 wh_uv;//uv width and height

//to gs
out VS_OUT_GS_IN
{
	vec2 wh;

	vec2 tex_coord;
	vec2 wh_uv;

} vs_out;

void main()
{
    gl_Position = transform * vec4(position, 0, 1);

    vs_out.wh = wh * wh_scale;

    vs_out.tex_coord    = tex_coord;
    vs_out.wh_uv        = wh_uv;
}