#version 450

layout(location = 0) out vec3 fragColor;

void main() {
    if (gl_VertexIndex == 0) {
        gl_Position = vec4(0.0, -0.5, 0.0, 1.0);
        fragColor = vec3(1.0, 0.0, 0.0);
    } else if (gl_VertexIndex == 1){
        gl_Position = vec4(0.5, 0.5, 0.0, 1.0);
        fragColor = vec3(0.0, 1.0, 0.0);
    } else if (gl_VertexIndex == 2){
        gl_Position = vec4(-0.5, 0.5, 0.0, 1.0);
        fragColor = vec3(0.0, 0.0, 1.0);
    }
}
