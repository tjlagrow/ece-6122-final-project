#version 410

in vec2 fcoord;

uniform sampler2D txtSampler;
uniform vec3 txtColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(txtSampler, fcoord).r);
    gl_FragColor = vec4(txtColor, 1.0) * sampled;
}