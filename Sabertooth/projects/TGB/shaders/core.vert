#version 410

layout(location=0) in vec3 vp;
layout(location=1) in vec3 vn;
layout(location=2) in vec2 vt;

uniform mat4 view, proj, model;

out vec2 texCoord;
out vec3 eye_position, eye_normal;

void main () {
    // armazena a posicao e normal para passar para o Fragment Shader
    eye_position = vec3(view * model * vec4(vp, 1.0));
    eye_normal = vec3(view * model * vec4(vn, 0.0));

    //vertices de textura
    texCoord = vt;

    //define glposition
     gl_Position = proj * vec4(eye_position, 1.0);
}