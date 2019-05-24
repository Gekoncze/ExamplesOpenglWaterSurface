#version 330

in vec3 vertex;
in vec2 uv;
uniform mat4 matrix;
out vec2 frag_uv;

void main()
{
    frag_uv = uv;
    gl_Position = matrix * vec4(vertex, 1.0);
}
