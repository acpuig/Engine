#version 330 core

uniform sampler2D mytexture;

out vec4 color;
in vec2 uv0;

void main()
{
 color = texture2D(mytexture, uv0);
}