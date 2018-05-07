//FRAGMENT SHADER
#version 330 core

uniform sampler2D tex;

//in vec2 fs_tex_coord; //left-most coord of texture on atlas, this is added to quad vertex
in vec2 fs_uv; //uv of individual quad vertex

out vec4 out_color;

void main()
{	
	//out_color = vec4(0, 1, 0, 1);

	vec2 new_uv = vec2(fs_uv.x, 1.0f-fs_uv.y);
    out_color = texture(tex, new_uv);

   	//out_color = vec4(new_uv, 0, 1);
}