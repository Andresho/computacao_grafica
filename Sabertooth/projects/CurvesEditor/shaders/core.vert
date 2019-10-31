#version 410

layout(location=0) in vec3 vp;

uniform mat4  proj;

void main () {
    //define glposition
    gl_PointSize = 10.0;
    gl_Position = proj * vec4(vp, 1.0);
}