//GEOMETRY SHADER
//expand left-most vertex to full quad, return tex_coords
#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT_GS_IN
{
    vec2 wh;

    vec2 tex_coord;
    vec2 wh_uv;

} gs_in[];

//in VS_OUT_GS_IN
//{
//    float width;
//    float height;
//
//    vec2 tex_coord;
//    float width_uv;
//    float height_uv;
//
//} gs_in[];


out vec2 fs_uv;


void main()
{
    vec4 position = gl_in[0].gl_Position;

    //ordering matters
    //bottom-left vertex

    gl_Position = position;
    fs_uv = vec2(0, 0) + gs_in[0].tex_coord;
    EmitVertex();

    //bottom-right vertex
    gl_Position = position + vec4(gs_in[0].wh.x, 0, 0, 0); //must be zero here...
    fs_uv = vec2(gs_in[0].wh_uv.x, 0) + gs_in[0].tex_coord;
    EmitVertex();

    //top-left vertex
    gl_Position = position + vec4(0, gs_in[0].wh.y, 0, 0);
    fs_uv = vec2(0, gs_in[0].wh_uv.y) + gs_in[0].tex_coord;
    EmitVertex();

    //top-right vertex
    gl_Position = position + vec4(gs_in[0].wh, 0, 0);
    fs_uv = gs_in[0].wh_uv + gs_in[0].tex_coord;
    EmitVertex();

    EndPrimitive();
}
