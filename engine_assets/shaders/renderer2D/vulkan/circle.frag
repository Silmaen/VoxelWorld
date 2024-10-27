#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput {
    vec3 LocalPosition;
    vec4 Color;
    float Thickness;
    float Fade;
};

layout (location = 0) in VertexOutput i_Vertex;
layout (location = 4) in flat int i_EntityID;

// convert color space to linear!
vec4 sRGBToLinear(vec4 srgbColor) {
    vec4 linearColor;
    // Convertir chaque composante de couleur sRGB en couleur linéaire
    linearColor.r = (srgbColor.r <= 0.04045) ? (srgbColor.r / 12.92) : pow((srgbColor.r + 0.055) / 1.055, 2.4);
    linearColor.g = (srgbColor.g <= 0.04045) ? (srgbColor.g / 12.92) : pow((srgbColor.g + 0.055) / 1.055, 2.4);
    linearColor.b = (srgbColor.b <= 0.04045) ? (srgbColor.b / 12.92) : pow((srgbColor.b + 0.055) / 1.055, 2.4);
    linearColor.a = srgbColor.a;
    return linearColor;
}

void main() {
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(i_Vertex.LocalPosition);
    float circle = smoothstep(0.0, i_Vertex.Fade, distance);
    circle *= smoothstep(i_Vertex.Thickness + i_Vertex.Fade, i_Vertex.Thickness, distance);

    if (circle == 0.0)
    discard;

    // Set output color
    o_Color = sRGBToLinear(i_Vertex.Color);
    o_Color.a *= circle;
    if (o_Color.a == 0)discard;

    o_EntityID = i_EntityID;
}
