#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput{
    vec4 Color;
    vec2 TexCoord;
};

layout (location = 0) in VertexOutput i_Vertex;
layout (location = 2) in flat float i_TexIndex;
layout (location = 3) in flat int i_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32];

float screenPxRange() {
    const float pxRange = 2.0;// set to distance field's pixel range
    vec2 unitRange;
    switch (int(i_TexIndex)) {
        case 0: unitRange = vec2(pxRange)/vec2(texture(u_Textures[0], i_Vertex.TexCoord)); break;
        case 1: unitRange = vec2(pxRange)/vec2(texture(u_Textures[1], i_Vertex.TexCoord)); break;
        case 2: unitRange = vec2(pxRange)/vec2(texture(u_Textures[2], i_Vertex.TexCoord)); break;
        case 3: unitRange = vec2(pxRange)/vec2(texture(u_Textures[3], i_Vertex.TexCoord)); break;
        case 4: unitRange = vec2(pxRange)/vec2(texture(u_Textures[4], i_Vertex.TexCoord)); break;
        case 5: unitRange = vec2(pxRange)/vec2(texture(u_Textures[5], i_Vertex.TexCoord)); break;
        case 6: unitRange = vec2(pxRange)/vec2(texture(u_Textures[6], i_Vertex.TexCoord)); break;
        case 7: unitRange = vec2(pxRange)/vec2(texture(u_Textures[7], i_Vertex.TexCoord)); break;
        case 8: unitRange = vec2(pxRange)/vec2(texture(u_Textures[8], i_Vertex.TexCoord)); break;
        case 9: unitRange = vec2(pxRange)/vec2(texture(u_Textures[9], i_Vertex.TexCoord)); break;
        case 10: unitRange = vec2(pxRange)/vec2(texture(u_Textures[10], i_Vertex.TexCoord)); break;
        case 11: unitRange = vec2(pxRange)/vec2(texture(u_Textures[11], i_Vertex.TexCoord)); break;
        case 12: unitRange = vec2(pxRange)/vec2(texture(u_Textures[12], i_Vertex.TexCoord)); break;
        case 13: unitRange = vec2(pxRange)/vec2(texture(u_Textures[13], i_Vertex.TexCoord)); break;
        case 14: unitRange = vec2(pxRange)/vec2(texture(u_Textures[14], i_Vertex.TexCoord)); break;
        case 15: unitRange = vec2(pxRange)/vec2(texture(u_Textures[15], i_Vertex.TexCoord)); break;
        case 16: unitRange = vec2(pxRange)/vec2(texture(u_Textures[16], i_Vertex.TexCoord)); break;
        case 17: unitRange = vec2(pxRange)/vec2(texture(u_Textures[17], i_Vertex.TexCoord)); break;
        case 18: unitRange = vec2(pxRange)/vec2(texture(u_Textures[18], i_Vertex.TexCoord)); break;
        case 19: unitRange = vec2(pxRange)/vec2(texture(u_Textures[19], i_Vertex.TexCoord)); break;
        case 20: unitRange = vec2(pxRange)/vec2(texture(u_Textures[20], i_Vertex.TexCoord)); break;
        case 21: unitRange = vec2(pxRange)/vec2(texture(u_Textures[21], i_Vertex.TexCoord)); break;
        case 22: unitRange = vec2(pxRange)/vec2(texture(u_Textures[22], i_Vertex.TexCoord)); break;
        case 23: unitRange = vec2(pxRange)/vec2(texture(u_Textures[23], i_Vertex.TexCoord)); break;
        case 24: unitRange = vec2(pxRange)/vec2(texture(u_Textures[24], i_Vertex.TexCoord)); break;
        case 25: unitRange = vec2(pxRange)/vec2(texture(u_Textures[25], i_Vertex.TexCoord)); break;
        case 26: unitRange = vec2(pxRange)/vec2(texture(u_Textures[26], i_Vertex.TexCoord)); break;
        case 27: unitRange = vec2(pxRange)/vec2(texture(u_Textures[27], i_Vertex.TexCoord)); break;
        case 28: unitRange = vec2(pxRange)/vec2(texture(u_Textures[28], i_Vertex.TexCoord)); break;
        case 29: unitRange = vec2(pxRange)/vec2(texture(u_Textures[29], i_Vertex.TexCoord)); break;
        case 30: unitRange = vec2(pxRange)/vec2(texture(u_Textures[30], i_Vertex.TexCoord)); break;
        case 31: unitRange = vec2(pxRange)/vec2(texture(u_Textures[31], i_Vertex.TexCoord)); break;
    }
    vec2 screenTexSize = vec2(1.0)/fwidth(i_Vertex.TexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec4 texColor = i_Vertex.Color;
    vec3 msd;
    switch (int(i_TexIndex)) {
        case 0: texColor *= texture(u_Textures[0], i_Vertex.TexCoord);msd = texture(u_Textures[0], i_Vertex.TexCoord).rgb; break;
        case 1: texColor *= texture(u_Textures[1], i_Vertex.TexCoord);msd = texture(u_Textures[1], i_Vertex.TexCoord).rgb; break;
        case 2: texColor *= texture(u_Textures[2], i_Vertex.TexCoord);msd = texture(u_Textures[2], i_Vertex.TexCoord).rgb; break;
        case 3: texColor *= texture(u_Textures[3], i_Vertex.TexCoord);msd = texture(u_Textures[3], i_Vertex.TexCoord).rgb; break;
        case 4: texColor *= texture(u_Textures[4], i_Vertex.TexCoord);msd = texture(u_Textures[4], i_Vertex.TexCoord).rgb; break;
        case 5: texColor *= texture(u_Textures[5], i_Vertex.TexCoord);msd = texture(u_Textures[5], i_Vertex.TexCoord).rgb; break;
        case 6: texColor *= texture(u_Textures[6], i_Vertex.TexCoord);msd = texture(u_Textures[6], i_Vertex.TexCoord).rgb; break;
        case 7: texColor *= texture(u_Textures[7], i_Vertex.TexCoord);msd = texture(u_Textures[7], i_Vertex.TexCoord).rgb; break;
        case 8: texColor *= texture(u_Textures[8], i_Vertex.TexCoord);msd = texture(u_Textures[8], i_Vertex.TexCoord).rgb; break;
        case 9: texColor *= texture(u_Textures[9], i_Vertex.TexCoord);msd = texture(u_Textures[9], i_Vertex.TexCoord).rgb; break;
        case 10: texColor *= texture(u_Textures[10], i_Vertex.TexCoord);msd = texture(u_Textures[10], i_Vertex.TexCoord).rgb; break;
        case 11: texColor *= texture(u_Textures[11], i_Vertex.TexCoord);msd = texture(u_Textures[11], i_Vertex.TexCoord).rgb; break;
        case 12: texColor *= texture(u_Textures[12], i_Vertex.TexCoord);msd = texture(u_Textures[12], i_Vertex.TexCoord).rgb; break;
        case 13: texColor *= texture(u_Textures[13], i_Vertex.TexCoord);msd = texture(u_Textures[13], i_Vertex.TexCoord).rgb; break;
        case 14: texColor *= texture(u_Textures[14], i_Vertex.TexCoord);msd = texture(u_Textures[14], i_Vertex.TexCoord).rgb; break;
        case 15: texColor *= texture(u_Textures[15], i_Vertex.TexCoord);msd = texture(u_Textures[15], i_Vertex.TexCoord).rgb; break;
        case 16: texColor *= texture(u_Textures[16], i_Vertex.TexCoord);msd = texture(u_Textures[16], i_Vertex.TexCoord).rgb; break;
        case 17: texColor *= texture(u_Textures[17], i_Vertex.TexCoord);msd = texture(u_Textures[17], i_Vertex.TexCoord).rgb; break;
        case 18: texColor *= texture(u_Textures[18], i_Vertex.TexCoord);msd = texture(u_Textures[18], i_Vertex.TexCoord).rgb; break;
        case 19: texColor *= texture(u_Textures[19], i_Vertex.TexCoord);msd = texture(u_Textures[19], i_Vertex.TexCoord).rgb; break;
        case 20: texColor *= texture(u_Textures[20], i_Vertex.TexCoord);msd = texture(u_Textures[20], i_Vertex.TexCoord).rgb; break;
        case 21: texColor *= texture(u_Textures[21], i_Vertex.TexCoord);msd = texture(u_Textures[21], i_Vertex.TexCoord).rgb; break;
        case 22: texColor *= texture(u_Textures[22], i_Vertex.TexCoord);msd = texture(u_Textures[22], i_Vertex.TexCoord).rgb; break;
        case 23: texColor *= texture(u_Textures[23], i_Vertex.TexCoord);msd = texture(u_Textures[23], i_Vertex.TexCoord).rgb; break;
        case 24: texColor *= texture(u_Textures[24], i_Vertex.TexCoord);msd = texture(u_Textures[24], i_Vertex.TexCoord).rgb; break;
        case 25: texColor *= texture(u_Textures[25], i_Vertex.TexCoord);msd = texture(u_Textures[25], i_Vertex.TexCoord).rgb; break;
        case 26: texColor *= texture(u_Textures[26], i_Vertex.TexCoord);msd = texture(u_Textures[26], i_Vertex.TexCoord).rgb; break;
        case 27: texColor *= texture(u_Textures[27], i_Vertex.TexCoord);msd = texture(u_Textures[27], i_Vertex.TexCoord).rgb; break;
        case 28: texColor *= texture(u_Textures[28], i_Vertex.TexCoord);msd = texture(u_Textures[28], i_Vertex.TexCoord).rgb; break;
        case 29: texColor *= texture(u_Textures[29], i_Vertex.TexCoord);msd = texture(u_Textures[29], i_Vertex.TexCoord).rgb; break;
        case 30: texColor *= texture(u_Textures[30], i_Vertex.TexCoord);msd = texture(u_Textures[30], i_Vertex.TexCoord).rgb; break;
        case 31: texColor *= texture(u_Textures[31], i_Vertex.TexCoord);msd = texture(u_Textures[31], i_Vertex.TexCoord).rgb; break;
    }
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    if (opacity == 0.0)
    discard;
    vec4 bgColor = vec4(0.0);
    o_Color = mix(bgColor, i_Vertex.Color, opacity);
    if (o_Color.a == 0.0)
    discard;

    o_EntityID = i_EntityID;
}
