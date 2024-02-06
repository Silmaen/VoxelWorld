#version 120

attribute vec3 a_Position;
attribute vec4 a_Color;
attribute float a_EntityID;

uniform mat4 u_ViewProjection;

varying vec4 v_Color;
varying float v_EntityID;

void main() {
    v_Color = a_Color;
    v_EntityID = a_EntityID;

    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}