#version 330

// Output
layout(location = 0) out vec4 out_color;

in vec3 frag_color;

void main()
{
    out_color = vec4(frag_color, 1);
}
