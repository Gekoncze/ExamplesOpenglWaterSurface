#version 330

in vec3 vertex;
in vec2 uv;
uniform mat4 matrix;
out vec2 frag_uv;
out float frag_y;

void main()
{
    frag_uv = uv;
	frag_y = vertex.y;
    gl_Position = matrix * vec4(vertex, 1.0);
}
