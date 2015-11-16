#version 330

uniform vec3 input_color;

out vec4 output_color;

void main()  
{
    output_color = vec4(input_color, 0.0);
}
