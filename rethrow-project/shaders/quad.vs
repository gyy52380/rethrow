//VERTEX SHADER
//position -> geometry shader
//width, height -> geometry shader
//uv -> fragment shader
#version 330 core

layout(location = 0) in vec2 position; //position of left-most quad vertex
layout(location = 1) in float width;   //quad widt and height
layout(location = 2) in float height;

layout(location = 3) in vec2 tex_coord;//left-most texture coord on the atlas
layout(location = 4) in float width_uv;   //uv widt and height
layout(location = 5) in float height_uv; 

//to gs
out float gs_width;
out float gs_height;
out float gs_width_uv;
out float gs_height_uv;

//to fs
out vec2 fs_tex_coord;

void main()
{
    gl_Position = vec4(position, 0, 1);

    gs_width = width;
    gs_height = height;
    fs_tex_coord = tex_coord; 
}