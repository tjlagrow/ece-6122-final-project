#version 410

in vec3 vpos;
in vec4 vcol;

out vec4 fcol;

uniform mat4 mvp;
uniform mat4 vpmat;
uniform mat4 vvmat;
uniform mat4 vmmat;

void main()
{
    gl_Position = mvp * vec4(vpos, 1.0);
    fcol = vcol;
}