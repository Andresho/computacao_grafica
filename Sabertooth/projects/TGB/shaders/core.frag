#version 330

in vec2 texCoord;
out vec4 frag_color;
uniform sampler2D tex;
uniform bool selected;

//Ka é coeficiente de reflexão do objeto
uniform vec3 Ka, Kd, Ks;
uniform float Ns;

//intensidade da luz ambiente
vec3 Ia = vec3 (0.2, 0.2, 0.2); // cor ambiente cinza escuro

void main () {
    // luz ambiente multiplicado pela refletância da superfície
    vec3 Ia = Ia * Ka;

	vec4 texel0 = texture(tex, texCoord);
	if(texel0.a < 0.5){
        discard;
    }
    if(selected){
        frag_color = texel0 * vec4(0.0,1.0,0.0,1.0);
    } else {
        frag_color = texel0;
    }
}