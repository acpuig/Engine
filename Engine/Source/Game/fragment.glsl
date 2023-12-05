#version 330
out vec4 color;

// in vec3 normal;
// in vec3 frag_world_position;
in vec2 tex_coord;

uniform sampler2D texture;

void main()
{
    color = texture2D(texture, tex_coord);

//    vec3 texture_color = texture(texture, tex_coord).xyz;;
//    color = texture(texture, tex_coord);
//	color = vec4(texture_color, 1.0);
}

