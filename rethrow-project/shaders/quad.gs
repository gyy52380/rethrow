//GEOMETRY SHADER
//expand left-most vertex to full quad, return tex_coords
#version 330 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in float gs_width;
in float gs_height;
in float gs_width_uv;
in float gs_height_uv;

out vec2 fs_relative_vertex_uv;

void main()
{
    vec4 position = gl_in[0].gl_Position;

    //bottom-left vertex
    gl_Position = position; //+ vec2(0, 0)
    fs_relative_vertex_uv = vec2(0, 0);
    EmitVertex();

    //top-left vertex
    gl_Position = position + vec2(0, gs_height);
    fs_relative_vertex_uv = vec2(0, gs_height_uv);
    EmitVertex();

    //top-right vertex
    gl_Position = position + vec2(gs_width, gs_height);
    fs_relative_vertex_uv = vec2(gs_width_uv, gs_height_uv);
    EmitVertex();

    //bottom-right vertex
    gl_Position = position + vec2(gs_width, 0);
    fs_relative_vertex_uv = vec2(gs_width_uv, 0);
    EmitVertex();

    EndPrimitive();
}
