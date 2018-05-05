//FRAGMENT SHADER
#version 330 core

uniform sampler2D tex_atlas;

in vec2 fs_tex_coord; //left-most coord of texture on atlas, this is added to quad vertex
in vec2 fs_relative_vertex_uv; //uv of individual quad vertex

out vec4 out_color;

void main()
{
    out_color = texture2D(tex_atlas, fs_tex_coord+fs_relative_vertex_uv);
}