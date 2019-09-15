#pragma warning(disable:4996)

#include "runCube.h"

#define WEIGTH 800
#define HEIGHT 600

bool keys[1024];

float xCentro = WEIGTH/2;
float yCentro = HEIGHT/2;
float value_scala = 1.2f;
float value_move = 0.10f;
float angle_rotation = 1.0;
float cameraSpeed = 3.f; // adjust accordingly
float fov = 45.0f;

float pitchAngle = 0.f;
float yawAngle = -90.f;

float directionSpeed = 50.f;

vector<Face*> faces;
vector<Group*> groups;
vector<Obj3d*> objs;
vector<Material*> materials;

glm::mat4 proj = glm::perspective(glm::radians(fov),((float)WEIGTH)/((float)HEIGHT),0.1f,100.0f);

//eye: posição
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
// direction: direção
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 direction = glm::vec3(
        glm::normalize(
                glm::vec3(
                        cos(glm::radians(pitchAngle)) * cos(glm::radians(yawAngle)),
                        sin(glm::radians(pitchAngle)),
                        cos(glm::radians(pitchAngle)) * sin(glm::radians(yawAngle))
)));

glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// right: direita. Vetor perpendicular ao plano direction-up
glm::vec3 cameraRight = glm::normalize(glm::cross(direction, cameraUp));

glm::mat4 view =glm::mat4(1);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 90.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 90.0f)
        fov = 90.0f;
}

void keyboard_reaction(float elapsedTime, bool &hasKeyboardPressed) {
    hasKeyboardPressed = keys[GLFW_KEY_W] || keys[GLFW_KEY_S] || keys[GLFW_KEY_A]
                         || keys[GLFW_KEY_D] || keys[GLFW_KEY_Q] || keys[GLFW_KEY_E]
                         || keys[GLFW_KEY_RIGHT] ||keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_DOWN]
                         || keys[GLFW_KEY_UP];
    if(!hasKeyboardPressed)
        return;

	// MECHE A POSICAO DA CAMERA
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * direction * elapsedTime;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * direction * elapsedTime;
	if (keys[GLFW_KEY_A])
		cameraPos -= cameraRight * cameraSpeed * elapsedTime;
	if (keys[GLFW_KEY_D])
		cameraPos += cameraRight * cameraSpeed * elapsedTime;
	if (keys[GLFW_KEY_Q])
		cameraPos -= cameraUp * cameraSpeed * elapsedTime;
	if (keys[GLFW_KEY_E])
		cameraPos += cameraUp * cameraSpeed * elapsedTime;

	//meche a direcao da camera
	if (keys[GLFW_KEY_RIGHT]) {
		yawAngle += directionSpeed * elapsedTime;

		cameraRight = cross(direction, cameraUp);
	}
	if (keys[GLFW_KEY_LEFT]) {
		yawAngle -= directionSpeed * elapsedTime;

		cameraRight = cross(direction, cameraUp);
	}

	if (keys[GLFW_KEY_DOWN]) {
		pitchAngle -= directionSpeed* elapsedTime;
		if (pitchAngle > 89.0f)
			pitchAngle = 89.0f;
		if (pitchAngle < -89.0f)
			pitchAngle = -89.0f;

//		cameraUp = cross(direction, cameraRight);
	}

	if (keys[GLFW_KEY_UP]) {
		pitchAngle += directionSpeed* elapsedTime;
		if (pitchAngle > 89.0f)
			pitchAngle = 89.0f;
		if (pitchAngle < -89.0f)
			pitchAngle = -89.0f;

		//cameraUp = cross(direction, cameraRight);
	}
	glm::vec3 front;
	front.x = cos(glm::radians(pitchAngle)) * cos(glm::radians(yawAngle));
	front.y = sin(glm::radians(pitchAngle));
	front.z = cos(glm::radians(pitchAngle)) * sin(glm::radians(yawAngle));
	direction = glm::normalize(front);

}

//Define acoes do teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) keys[key] = true;
	if (action == GLFW_RELEASE) keys[key] = false;
}

bool convertFileInString(const char *fileName, char *shaderStr, int maxLength) {
    shaderStr[0] = '\0'; // zera a string
    FILE *file = fopen(fileName, "r");
    if ( !file ) {
        return false;
    }
    int currentLen = 0;
    char line[2048];
    strcpy(line, ""); // limpa a linha
    while (!feof(file)) {
        if (nullptr != fgets( line, 2048, file )) {
            currentLen += strlen( line );
            strcat( shaderStr, line );
        }
    }
    if (EOF == fclose(file)) {
        return false;
    }
    return true;
}

void readMaterial(string pathName,string fileName){
    Material *material;

    ifstream arq(pathName+fileName);
    if (!arq){
        cout << "Archive not found!" + fileName;
    } else {
        while (!arq.eof()) {

            string line;
            getline(arq, line);
            stringstream sline;
            sline << line;
            string temp;
            sline >> temp; // pega ate primeiro espaco

            if (temp == "newmtl") {
                string nameMTL;
                sline >> nameMTL;
                material = new Material();
                material->materialName = nameMTL;
                materials.push_back(material);
            }
            else if (temp == "map_Kd") {
                string nameTexture;
                sline >> nameTexture;
                string pathWithName = pathName+nameTexture;
                materials[materials.size()-1]->createTexture(pathWithName.c_str(),false);
            }
        }

    }
}

Mesh* readData(string pathName,string fileName) {
    Mesh *mesh = new Mesh();
    Group *g = nullptr;

    vec3 maxBoundBox = vec3(0.f,0.f,0.f);
	vec3 minBoundBox = vec3(0.f,0.f,0.f);

    ifstream arq(pathName+fileName);

    if (!arq){
        cout << "Archive not found!" + fileName;
    } else {
        while (!arq.eof()) {
            string line;
            getline(arq, line);
            stringstream sline;
            sline << line;
            string temp;
            sline >> temp; // pega ate primeiro espaco
            if (temp == "mtllib") {
                string fName;
                sline >> fName;
                readMaterial(pathName,fName);
            }
            else if (temp == "v") {
                // ler vértice ...
                float x, y, z;
                sline >> x >> y >> z;
                // atribuir vértices da malha
                if(x>maxBoundBox.x)
                    maxBoundBox.x = x;
                if(y>maxBoundBox.y)
                    maxBoundBox.y = y;
                if(z>maxBoundBox.z)
                    maxBoundBox.z = z;

                if(x<minBoundBox.x)
                    minBoundBox.x = x;
                if(y<minBoundBox.y)
                    minBoundBox.y = y;
                if(z<minBoundBox.z)
                    minBoundBox.z = z;
                vec3* v = new vec3(x, y, z);
                mesh->vertex.push_back(v);
            }
            else if (temp == "vn") {
                // ler normais ...
                float x, y, z;
                sline >> x >> y >> z;
                //atribuir normais da malha
                vec3* vn = new vec3(x, y, z);
                mesh->normals.push_back(vn);
            }
            else if (temp == "vt") {
                // ler mapeamento de textura ...
                float x, y;
                sline >> x >> y;
                // atribuir mapeamentos de textura da malha
                vec2* vt = new vec2(x, y);
                mesh->mappings.push_back(vt);
            }
            else if (temp == "g"||temp == "s") {
                if (g != nullptr) {
                    mesh->groups.push_back(g);
                }
                string gName;
                sline >> gName;
                g = new Group(gName, "default");
            } else if (temp == "f") {
                //se grupo nao existe, cria um padrao
                if (g == nullptr) {
                    g = new Group("default", "default");
                }
                // implementar lógica de varições
                // para face: v, v/t/n, v/t e v//n
                Face *f = new Face();
                Face *fAux = new Face();
                int vertices = 0;

                // le todas faces ate final da linha
                while(!sline.eof()) {
                    vertices++;
                    string token;
                    sline >> token;
                    if (token.empty()) {
                        continue;
                    }
                    stringstream stoken;
                    stoken << token;

                    string aux[3];
                    int countParamsFace = -1;
                    do {
                        countParamsFace = countParamsFace + 1;
                        getline(stoken, aux[countParamsFace], '/');
                    } while (!stoken.eof());

                    for (int i = 0; i < 3; i = i + 1) {
                        switch (i) {
                            // posicao 0 --> indice de vertex
                            case 0:
                                if (aux[i].empty()) {
                                    if(vertices>3)
                                        fAux->verts.push_back(-1);
                                    else
                                        f->verts.push_back(-1);
                                } else {
                                    if(vertices>3)
                                        fAux->verts.push_back(stoi(aux[i])-1);
                                    else
                                        f->verts.push_back(stoi(aux[i])-1);
                                }
                                break;
                                // posicao 1 --> indice de mapamento de textura
                            case 1:
                                if (aux[i].empty()) {
                                    if(vertices>3)
                                        fAux->texts.push_back(-1);
                                    else
                                        f->texts.push_back(-1);
                                } else {
                                    if(vertices>3)
                                        fAux->texts.push_back(stoi(aux[i])-1);
                                    else
                                        f->texts.push_back(stoi(aux[i])-1);
                                }
                                break;
                                // posicao 1 --> indice de normais
                            case 2:
                                if (aux[i].empty()) {
                                    if(vertices>3)
                                        fAux->norms.push_back(-1);
                                    else
                                        f->norms.push_back(-1);
                                } else {
                                    if(vertices>3)
                                        fAux->norms.push_back(stoi(aux[i])-1);
                                    else
                                        f->norms.push_back(stoi(aux[i])-1);
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }

                // Adiciona face no grupo
                g->faces.push_back(f);
                if(vertices>3){
                    for(int i=0; i<2; i++){
                        fAux->verts.push_back(f->verts[i]);
                        fAux->norms.push_back(f->norms[i]);
                        fAux->texts.push_back(f->texts[i]);
                    }
                    // Adiciona face no grupo
                    g->faces.push_back(fAux);
                }

            }
        }
        // adiciona grupo para o mesh
        mesh->groups.push_back(g);
        mesh->calculeDistanceScale(maxBoundBox,minBoundBox);
        return mesh;
    }
    return nullptr;
}

void loadVertices(Mesh* mesh) {
    Group *g;
    Face *f;
    for (int i = 0; i < mesh->groups.size(); i++) {
        g = mesh->groups[i];

        vector<float> vs;
        vector<float> vts;
        vector<float> vns;

        for (int j = 0; j < g->faces.size(); j++) {
            f = g->faces[j];

            // para vertices
            for(int k = 0; k< (f->verts.size()); k++){
                int ind_v = f->verts[k];
                if(ind_v>-1){
                    vec3* v = mesh->vertex[ind_v];
                    vs.push_back(v->x);
                    vs.push_back(v->y);
                    vs.push_back(v->z);
                }
            }

            // para normais
            for(int n = 0; n<(f->norms.size()); n++){
                int ind_n = f->norms[n];
                if(ind_n>-1) {
                    vec3 *vn = mesh->normals[ind_n];
                    vns.push_back(vn->x);
                    vns.push_back(vn->y);
                    vns.push_back(vn->z);
                }
            }

            // para texturas
            for(int t = 0; t< (f->texts.size()); t++){
                int ind_t = f->texts[t];
                if(ind_t>-1) {
                    vec2 *vt = mesh->mappings[ind_t];
                    vts.push_back(vt->x);
                    vts.push_back(vt->y);
                }
            }
        }
        g->numVertices = vs.size()/3;

        float*  arrayPoints = vs.data();
        float*  arrayNormals = vns.data();
        float*  arrayTexts = vts.data();

        /*
        VAO
        */
        glGenVertexArrays(1, &g->vao);
        glBindVertexArray(g->vao);

        /*
        VBO vertices
        */
        GLuint vboVerts;
        glGenBuffers(1, &vboVerts);
        glBindBuffer(GL_ARRAY_BUFFER, vboVerts);
        glBufferData(GL_ARRAY_BUFFER, vs.size() * sizeof(float), arrayPoints, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);// habilitado primeiro atributo do shader
        // associacao do vbo atual com primeiro atributo
        // 0 identifica que o primeiro atributo está sendo definido
        // 3, GL_FLOAT identifica que dados sao vec3 e estao a cada 3 float.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        /*
            VBO normals
        */
        GLuint vboNorms;
        glGenBuffers (1, &vboNorms);
        glBindBuffer (GL_ARRAY_BUFFER, vboNorms);
        glBufferData (GL_ARRAY_BUFFER, vns.size() * sizeof (float), arrayNormals, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        /*
            VBO textures
        */
        GLuint vboTexts;
        glGenBuffers (1, &vboTexts);
        glBindBuffer (GL_ARRAY_BUFFER, vboTexts);
        glBufferData (GL_ARRAY_BUFFER, vts.size() * sizeof (float), arrayTexts, GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);


    }
}


int runCube() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    /* Caso necess�rio, defini��es espec�ficas para SOs, p. e. Apple OSX *
    Definir como 3.2 para Apple OS X */

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //*/


    GLFWwindow* window = glfwCreateWindow(
            WEIGTH, HEIGHT, "Cube 3D", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    // inicia manipulador da extens�o GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    //habilita profundidade
    glEnable(GL_DEPTH_TEST);

    // obten��o de vers�o suportada da OpenGL e renderizador
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL (versao suportada) %s\n", version);
    // encerra contexto GL e outros recursos da GLFW

    /*
        Realiza a leitura dos dados para criar o Mesh
    */
//    Mesh* mesh = readData("projects/meshCube/objs/cube.obj");
    Mesh* mesh = readData("projects/meshCube/objs/mesa/","mesa01.obj");
//    Mesh* mesh = readData("projects/meshCube/objs/paintball/cenaPaintball.obj");

    // indica como ler os vertices
    loadVertices(mesh);

	objs.push_back(new Obj3d(mesh, 0, 0, 0));
	objs[0]->scale(1/mesh->distanceScale);
//	objs.push_back(new Obj3d(mesh, 2, 0, 0));
//	objs.push_back(new Obj3d(mesh, 0, 0, 6));

	// criacao dos shaders
    char vertex_shader[1024 * 256];
    char fragment_shader[1024 * 256];
    convertFileInString("projects/meshCube/shaders/core.vert", vertex_shader, 1024 * 256);
    convertFileInString("projects/meshCube/shaders/core.frag", fragment_shader, 1024 * 256);

    // identifica vShader e o associa com vertex_shader
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    auto vShaderPointer = (const GLchar *) vertex_shader;
    glShaderSource(vShader, 1, &vShaderPointer, NULL);
    glCompileShader(vShader);

    // identifica fShader e o associa com fragment_shader
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fShaderPointer = (const GLchar *) fragment_shader;
    glShaderSource(fShader, 1, &fShaderPointer, NULL);
    glCompileShader(fShader);

    // identifica do programa, adiciona partes e faz "linkagem"
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fShader);
    glAttachShader(shader_programme, vShader);
    glLinkProgram(shader_programme);












	/**********************************************************************************************************************/
	/*camera shader, vbo e vao.  Para desenhar pequenos vetores na origem que simbolizam a camera*/
	/**********************************************************************************************************************/
	
	GLuint camVao;
	glGenVertexArrays(1, &camVao);
	glBindVertexArray(camVao);

	float camVerts[18] = {
		cameraPos.x, cameraPos.y, cameraPos.z,
		direction.x, direction.y, direction.z,
		cameraPos.x, cameraPos.y, cameraPos.z,
		cameraUp.x + cameraPos.x, cameraUp.y + cameraPos.y, cameraUp.z + cameraPos.z,
		cameraPos.x, cameraPos.y, cameraPos.z,
		cameraRight.x + cameraPos.x, cameraRight.y + cameraPos.y, cameraRight.z + cameraPos.z
	};

	GLuint camVbo;
	glGenBuffers(1, &camVbo);
	glBindBuffer(GL_ARRAY_BUFFER, camVbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), camVerts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	/*camera shader*/
	
	const char* cam_vertex_shader =
		"#version 330"
		"layout(location = 0) in vec3 vp;"
		"void main() {"
		"	gl_Position = vec4(3*vp.x, 3*vp.y, 3*vp.z, 1.0);"
		"}";

	const char* cam_fragment_shader =
		"#version 330"
		"out vec4 frag_color;"
		"void main() {"
		"	frag_color = vec4(1.0, 1.0, 1.0, 1.0);"
		"}";

	//convertFileInString("projects/meshCube/shaders/camera.vert", cam_vertex_shader, 1024 * 256);
	//convertFileInString("projects/meshCube/shaders/camera.frag", cam_fragment_shader, 1024 * 256);
	
	// identifica vShader e o associa com vertex_shader
	GLuint camVShader = glCreateShader(GL_VERTEX_SHADER);
	//auto camVShaderPointer = (const GLchar *)cam_vertex_shader;
	glShaderSource(camVShader, 1, &cam_vertex_shader, NULL);
	glCompileShader(camVShader);

	// identifica fShader e o associa com fragment_shader
	GLuint camFShader = glCreateShader(GL_FRAGMENT_SHADER);
	//auto camFShaderPointer = (const GLchar *)cam_fragment_shader;
	glShaderSource(camFShader, 1, &cam_fragment_shader, NULL);
	glCompileShader(camFShader);

	// identifica do programa, adiciona partes e faz "linkagem"
	GLuint cam_shader_programme = glCreateProgram();
	glAttachShader(cam_shader_programme, camFShader);
	glAttachShader(cam_shader_programme, camVShader);
	glLinkProgram(cam_shader_programme);
	
	/**********************************************************************************************************************/
	/*fim camera shader*/
	/**********************************************************************************************************************/














    glUseProgram(shader_programme);

    int viewLocation = glGetUniformLocation(shader_programme, "view");
    int projLocation = glGetUniformLocation(shader_programme, "proj");
    int modelLocation = glGetUniformLocation(shader_programme, "model");

    //logs dos shaders
    int maxLength = 2048;
    int currentLength = 0;
    char logVS[2048];
    glGetShaderInfoLog(vShader, maxLength, &currentLength, logVS);
    printf("\nInformações de logs - Vertex Shader:\n%s\n", logVS);

    char logFS[2048];
    glGetShaderInfoLog(fShader, maxLength, &currentLength, logFS);
    printf("Informações de logs - Fragment Shader:\n%s\n", logFS);

    char logSP[2048];
    glGetProgramInfoLog(shader_programme, maxLength, &currentLength, logSP);
    printf("Informações de logs - Shader Program:\n%s", logSP);

    //define calbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

	glm::vec3 front = vec3(0);
	front.x = cos(glm::radians(pitchAngle)) * cos(glm::radians(yawAngle));
	front.y = sin(glm::radians(pitchAngle));
	front.z = cos(glm::radians(pitchAngle)) * sin(glm::radians(yawAngle));
	direction = glm::normalize(front);

    // passa projection and view ao shader
	view = glm::lookAt(cameraPos, cameraPos + direction, cameraUp);
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    glClearColor(0.4f, 0.278f, 0.184f, 0.0f);

	double previous_seconds = glfwGetTime();
    bool hasKeyboardPressed = false;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		keyboard_reaction(elapsed_seconds, hasKeyboardPressed);

		proj = glm::perspective(glm::radians(fov), ((float)WEIGTH) / ((float)HEIGHT), 0.1f, 100.0f);
//        view = glm::lookAt(cameraPos, cameraTarget, up);
        view = glm::lookAt(cameraPos, cameraPos + direction, cameraUp);
 //       view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

        //pass uniform location to shader
        if(hasKeyboardPressed)
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));


		glUseProgram(shader_programme);
		for (int o = 0; o < objs.size(); o++) {
			Group *g;
			for (int i = 0; i < objs[o]->mesh->groups.size(); i++) {
				g = mesh->groups[i];
				// Define vao como verte array atual
				glBindVertexArray(g->vao);

				//pass uniform location to shader
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(objs[o]->transformations));


//				glDrawArrays(GL_LINE_LOOP, 0,g->numVertices);
				glDrawArrays(GL_TRIANGLES, 0, g->numVertices);
			}
		}



		/*
			desenhar pequenos vetores na horigem que simbolizam a camera
		*/
		glUseProgram(cam_shader_programme);
		glBindVertexArray(camVao);
		glDrawArrays(GL_LINES, 0, 6);
		
		float camVertsNew[18] = {
			cameraPos.x, cameraPos.y, cameraPos.z,
			direction.x, direction.y, direction.z,
			cameraPos.x, cameraPos.y, cameraPos.z,
			cameraUp.x + cameraPos.x, cameraUp.y + cameraPos.y, cameraUp.z + cameraPos.z,
			cameraPos.x, cameraPos.y, cameraPos.z,
			cameraRight.x + cameraPos.x, cameraRight.y + cameraPos.y, cameraRight.z + cameraPos.z
		};

		glBindBuffer(GL_ARRAY_BUFFER, camVbo);
		glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), camVertsNew, GL_STATIC_DRAW);

        glfwSwapBuffers(window);

        /* para a janela 'responder' */
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}