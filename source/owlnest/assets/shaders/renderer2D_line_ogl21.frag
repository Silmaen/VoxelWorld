#version 120

varying vec4 v_Color;
varying float v_EntityID;

void main() {
    gl_FragData[0] = v_Color;
    // no frame buffer
    //gl_FragData[1] = vec4(v_EntityID, 0, 0, 0);// placeholder for our entity ID
}