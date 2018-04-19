#version 410

layout (location = 0) out vec4 finalColor;

in DATA
{
    vec4 pos;
    vec2 uv;
    vec4 col;
    vec3 norm;
} fin;

void main()
{
    finalColor = fin.col;
}