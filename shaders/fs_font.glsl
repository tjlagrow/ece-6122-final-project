#version 410

in vec2 fcoord;

out vec4 outColor;

uniform sampler2D sampler;
uniform vec4 inColor;

void main()
{
    outColor = vec4(1.0, 1.0, 1.0, texture2D(sampler, fcoord).r) * inColor;
}