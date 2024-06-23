#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput {
    vec4 Color;
};

layout (location = 0) in VertexOutput Input;
layout (location = 1) in flat int v_EntityID;

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
    o_Color = sRGBToLinear(Input.Color);
    o_EntityID = v_EntityID;
}