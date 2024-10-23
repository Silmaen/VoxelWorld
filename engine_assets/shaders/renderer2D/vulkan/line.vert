#version 450 core

layout(location = 0) in vec3 i_Position;
layout(location = 1) in vec4 i_Color;
layout(location = 2) in int i_EntityID;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjection;
};

struct VertexOutput {
    vec4 Color;
};

layout (location = 0) out VertexOutput o_Vertex;
layout (location = 1) out flat int o_EntityID;

void main() {
    o_Vertex.Color = i_Color;
    o_EntityID = i_EntityID;

    gl_Position = u_ViewProjection * vec4(i_Position, 1.0);
}
