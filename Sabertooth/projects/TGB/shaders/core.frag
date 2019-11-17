#version 410

//entradas
in vec2 texCoord;
in vec3 eye_position, eye_normal;

//uniforms
// uniform de textura
uniform sampler2D tex;
// uniform de objeto selecionado ou nao
uniform bool selected;

//define iluminacoes ativas
//0 para inativo e 1 ativo
uniform vec3 ilumination_Ia_Id_Is;

//Ka é coeficiente de reflexão do objeto
uniform vec3 Ka, Kd, Ks;
uniform float Ns;
uniform mat4 view;

//saidas
out vec4 frag_color;

// fonte de luz do mundo (lampada
vec3 light_position_world  = vec3 (0.0f, 2.0f, 0.0f);
vec3 La = vec3 (0.2, 0.2, 0.2); // Cor ambiente perto do bem escuro
vec3 Ld = vec3 (0.8, 0.8, 0.8); // branco opaco, cor clara e difusa
vec3 Ls = vec3 (1.0, 1.0, 1.0); // cor branca p/ especular

void main () {
    // luz do ambiente vezes a refletancia da supercie
    vec3 Ia = La * Ka;

    vec3 eye_normal_normalized = normalize(eye_normal);

    // Elevar a fonte de luz para o espaço de visao
//    vec3 light_position_eye = vec3 (view * vec4 (light_position_world, 1.0));
    vec3 light_position_eye = vec3 (vec4 (light_position_world, 1.0));

    // Mede a distancia da fonte da luz em relacao ao olho
    vec3 distance_to_light_eye = light_position_eye - eye_position;

    // direcao do olho em relacao a fonte de luz
    vec3 direction_to_light_eye = normalize (distance_to_light_eye);

    // produto escalar (angulo entre dois vetores)
    float dot_prod = dot (direction_to_light_eye, eye_normal_normalized);

    // caso produto escalar for negativo, define 0 como valor minimo
    dot_prod = max(dot_prod, 0.0);

    // Define  a contribuicao da difusa (intensidade final da difusa)
    vec3 Id = Ld * Kd * dot_prod;

    // calculo de intensidade da especular
    // Normaliza a posicao do olho em realacao a superficie
    vec3 surface_to_viewer_eye = normalize(-eye_position);

    vec3 half_way_eye = normalize (surface_to_viewer_eye + direction_to_light_eye);

    // produto escalar (angulo entre dois vetores)
    float dot_prod_specular = dot(half_way_eye, eye_normal_normalized);
    dot_prod_specular = max (dot (half_way_eye, eye_normal_normalized), 0.0);

    float specular_factor = pow (dot_prod_specular, Ns);

    // Define a intesidade final da especular
    vec3 Is = Ls * Ks * specular_factor;

    // textura
    vec4 texel0 = texture(tex, texCoord);

    // iluminacoes ativas
    Ia = ilumination_Ia_Id_Is.x * Ia;
    Id = ilumination_Ia_Id_Is.y * Id;
    Is = ilumination_Ia_Id_Is.z * Is;

    //contribuicao iluminacao
    vec4 color = vec4 (Ia + Id + Is, 1.0);

    // descart realizado para imagens png p/ posibilitar transparencia
	if(texel0.a < 0.5){
        discard;
    }

    //saida para tela
    frag_color = texel0 * color;

}