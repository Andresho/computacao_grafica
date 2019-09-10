#version 330

layout(location=0) in vec3 vp;
layout(location=1) in vec3 vn;
layout(location=2) in vec2 vt;
uniform mat4 view, proj, model;
out vec3 color;

void main () {
 color = vn;
 gl_Position = proj * view  * model * vec4(vp, 1.0);
}