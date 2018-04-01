#version 410

varying vec2 fcoord;
uniform sampler2D sampler;
uniform vec4 ucolor;

void main()
{
    gl_FragColor = vec4(1.0, 1.0, 1.0, texture(sampler, fcoord).r) * ucolor;
}