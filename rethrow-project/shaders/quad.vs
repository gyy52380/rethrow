//VERTEX SHADER
//position -> geometry shader
//width, height -> geometry shader
//uv -> fragment shader
#version 330 core

uniform mat4 projection;
uniform vec2 wh_scale;


layout(location = 0) in vec2 position; //position of left-most quad vertex
layout(location = 1) in vec2 wh;   //quad widt and height

layout(location = 2) in vec2 tex_coord;//left-most texture coord on the atlas
layout(location = 3) in vec2 wh_uv;//uv width and height

//to gs
out VS_OUT_GS_IN
{
	float width;
	float height;

	vec2 tex_coord;
	float width_uv;
	float height_uv;

} vs_out;


void main()
{
    gl_Position = projection * vec4(position, 0, 1);

    vs_out.width    = wh.x * wh_scale.x;
    vs_out.height   = wh.y * wh_scale.y;

    vs_out.tex_coord    = tex_coord; //invert y coords in gs
    vs_out.width_uv     = wh_uv.x;
    vs_out.height_uv    = wh_uv.y;
}