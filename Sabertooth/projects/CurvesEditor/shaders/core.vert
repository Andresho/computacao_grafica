#version 410

layout(location=0) in vec3 vp;

uniform mat4  proj;

out float colorIntensity;

void main () {

    colorIntensity = vp.z;

    //define point size
    gl_PointSize = 10.0;

    //define glposition
    gl_Position = proj * vec4(vp.x,vp.y,0.0, 1.0);

}