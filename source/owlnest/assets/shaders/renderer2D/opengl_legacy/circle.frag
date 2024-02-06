#version 120

varying vec3 v_LocalPosition;
varying vec4 v_Color;
varying float v_Thickness;
varying float v_Fade;
varying float v_EntityID;

void main() {
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(v_LocalPosition);
    float circle = smoothstep(0.0, v_Fade, distance);
    circle *= smoothstep(v_Thickness + v_Fade, v_Thickness, distance);

    if (circle == 0.0)
    discard;

    // Set output color
    gl_FragData[0] = vec4(v_Color.r, v_Color.g, v_Color.b, v_Color.a * circle);

    // No frame buffer
    //gl_FragData[1] = vec4(v_EntityID, 0, 0, 0);// placeholder for our entity ID
}