#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec3 v_Position;
out vec4 v_Color;

void main() {
    v_Position = position;
    v_Color = a_Color;
    gl_Position = vec4(position, 1.0);
}
