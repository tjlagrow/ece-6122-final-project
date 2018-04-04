#version 410

in vec4 vcoord;
out vec2 fcoord;

void main()
{
    gl_Position = vec4(vcoord.xy, 0.0, 1.0);
    fcoord = vcoord.zw;
}
