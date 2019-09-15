#version 330

in vec2 texCoord;
out vec4 frag_color;
uniform sampler2D tex;

void main () {
	vec4 texel0 = texture(tex, texCoord);
	if(texel0.a < 0.5){
        discard;
    }
 frag_color = texel0;
}