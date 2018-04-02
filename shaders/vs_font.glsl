#version 410

in vec4 vcoord;
out vec2 fcoord;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vcoord.xy, 0.0, 1.0);
    fcoord = vcoord.zw;
}