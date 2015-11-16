#version 330

uniform mat4 worldtoclip;

layout(location = 0) in vec3 position;

void main()
{
    gl_Position = worldtoclip * vec4(position, 1.0);
}
