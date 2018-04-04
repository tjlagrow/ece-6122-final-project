#version 410

varying vec2 texcoord;

uniform sampler2D sampler;
uniform vec4 color;

void main()
{
    gl_FragColor = vec4(1.0, 1.0, 1.0, texture2D(sampler, texcoord).r) * color;
}