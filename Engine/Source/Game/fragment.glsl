#version 330
out vec4 color;

in vec3 normal;
in vec3 frag_world_position;
//in vec2 tex_coord;

uniform sampler2D texture;

void main()
{
    // vec3 texture_color = texture2D(texture, tex_coord).xyz;
    // color = vec4(texture_color.xyz, texture2D(texture, tex_coord).w);
    color = texture2D(texture, uv0);
}

