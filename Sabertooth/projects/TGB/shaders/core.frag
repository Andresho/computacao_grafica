#version 330

in vec2 texCoord;
out vec4 frag_color;
uniform sampler2D tex;
uniform bool selected;

void main () {
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