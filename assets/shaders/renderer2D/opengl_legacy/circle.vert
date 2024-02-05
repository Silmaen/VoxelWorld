#version 120

attribute vec3 a_WorldPosition;
attribute vec3 a_LocalPosition;
attribute vec4 a_Color;
attribute float a_Thickness;
attribute float a_Fade;
attribute float a_EntityID;

uniform mat4 u_ViewProjection;

varying vec3 v_LocalPosition;
varying vec4 v_Color;
varying float v_Thickness;
varying float v_Fade;
varying float v_EntityID;

void main() {
    v_LocalPosition = a_LocalPosition;
    v_Color = a_Color;
    v_Thickness = a_Thickness;
    v_Fade = a_Fade;

    v_EntityID = a_EntityID;

    gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}