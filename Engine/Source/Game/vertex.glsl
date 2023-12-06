#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 tex_coord;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
    tex_coord = in_tex_coord;

}
