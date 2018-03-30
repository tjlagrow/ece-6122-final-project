#version 410

in vec3 vpos;
in vec4 vcol;

out vec4 fcol;

uniform mat4 transform_matrix;

void main()
{
    gl_Position = transform_matrix * vec4(vpos, 1.0);
    fcol = vcol;
}