#version 410

layout (location = 0) in vec4 vpos;
layout (location = 1) in vec2 vuv;
layout (location = 2) in vec4 vcol;
layout (location = 3) in vec3 vnorm;

out DATA
{
    vec4 pos;
    vec2 uv;
    vec4 col;
    vec3 norm;
} vout;

uniform mat4 vpmat = mat4(1.0);
uniform mat4 vvmat = mat4(1.0);

void main()
{
    vec4 pos = vpmat * vvmat * vpos;

    gl_Position = pos;

    vout.pos = pos;
    vout.uv = vuv;
    vout.col = vcol;
    vout.norm = vnorm;
}