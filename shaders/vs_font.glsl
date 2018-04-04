#version 410

attribute vec4 vcoord;
varying vec2 texcoord;

void main()
{
    gl_Position = vec4(vcoord.xy, 0.0, 1.0);
    texcoord = vcoord.zw;
}