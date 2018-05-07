//VERTEX SHADER
//position -> geometry shader
//width, height -> geometry shader
//uv -> fragment shader
#version 330 core

layout(location = 0) in vec2 position; //position of left-most quad vertex
layout(location = 1) in float width;   //quad widt and height
layout(location = 2) in float height;

layout(location = 3) in vec2 tex_coord;//left-most texture coord on the atlas
layout(location = 4) in float width_uv;//uv width and height
layout(location = 5) in float height_uv; 

//to gs

out VS_OUT_GS_IN
{
	float width;
	float height;

	vec2 tex_coord;
	float width_uv;
	float height_uv;

} vs_out;

//to fs
//out vec2 fs_tex_coord;

void main()
{
    gl_Position = vec4(position, 0, 1);

    vs_out.width = width;
    vs_out.height = height;

    vs_out.tex_coord = vec2(tex_coord.x, tex_coord.y); //invert y coords 
    vs_out.width_uv = width_uv;
    vs_out.height_uv = height_uv;
}