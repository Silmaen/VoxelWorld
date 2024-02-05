#version 120

attribute vec3 a_Position;
attribute vec4 a_Color;
attribute vec2 a_TexCoord;
attribute float a_TexIndex;
attribute float a_TilingFactor;
attribute float a_EntityID;

uniform mat4 u_ViewProjection;

varying vec4 v_Color;
varying vec2 v_TexCoord;
varying float v_TilingFactor;
varying float v_TexIndex;
varying float v_EntityID;

void main() {
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TilingFactor = a_TilingFactor;
    v_TexIndex = a_TexIndex;
    v_EntityID = a_EntityID;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
