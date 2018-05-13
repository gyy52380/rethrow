//FRAGMENT SHADER

#version 330 core

uniform sampler2D tex;

in VS_OUT_FS_IN
{
	vec2 uv;
	vec4 color;

} fs_in;

out vec4 frag_color;

void main()
{
	frag_color = texture(tex, fs_in.uv) * fs_in.color;
}