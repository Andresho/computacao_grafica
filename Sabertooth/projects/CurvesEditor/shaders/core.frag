#version 410

//entradas
in float colorIntensity;

//saidas
out vec4 frag_color;

void main () {

    //contribuicao iluminacao
    vec4 color = vec4 (colorIntensity, colorIntensity ,0.5, 1.0);

    //saida para tela
    frag_color = color;

}