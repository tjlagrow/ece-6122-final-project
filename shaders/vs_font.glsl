#version 410

attribute vec4 vcoord;
varying vec2 fcoord;

void main()
{
    gl_Position = vec4(vcoord.xy, 0, 1);
    fcoord = vcoord.zw;
}