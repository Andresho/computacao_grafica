#include "run.h"

vector<Face*> faces;
vector<Group*> groups;
Mesh *mesh;

glm::mat4 matrix_translaction = glm::mat4(1);
glm::mat4 matrix_rotation = glm::mat4(1);
glm::mat4 matrix_scala = glm::mat4(1);
glm::mat4 matrix = matrix_translaction * matrix_rotation * matrix_scala;

float xCentro = 0.0f;
float yCentro = 0.0f;
float value_scala = 1.2f;
float value_move = 3.0f;
float angle_rotation = 1.0;
//Define acoes do teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if ((action == GLFW_REPEAT || action == GLFW_PRESS)) {
        if (key == GLFW_KEY_U) {
            matrix_rotation = glm::rotate(matrix_rotation, glm::radians(-angle_rotation), glm::vec3(1, 0, 0));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
        }
        else if (key == GLFW_KEY_R) {
            matrix_rotation = glm::rotate(matrix_rotation, glm::radians(angle_rotation), glm::vec3(1, 0, 0));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
        }

        if (key == GLFW_KEY_I) {
            matrix_rotation = glm::rotate(matrix_rotation, glm::radians(-angle_rotation), glm::vec3(0, 1, 0));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
        }
        else if (key == GLFW_KEY_E) {
            matrix_rotation = glm::rotate(matrix_rotation, glm::radians(angle_rotation), glm::vec3(0, 1, 0));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
        }

        if (key == GLFW_KEY_O) {
            matrix_rotation = glm::rotate(matrix_rotation, glm::radians(-angle_rotation), glm::vec3(0, 0, 1));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
        }
        else if (key == GLFW_KEY_W) {
            matrix_rotation = glm::rotate(matrix_rotation, glm::radians(angle_rotation), glm::vec3(0, 0, 1));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
        }

        else if (key == GLFW_KEY_RIGHT) {
            matrix_translaction = glm::translate(matrix_translaction,
                                                     glm::vec3(value_move, 0.0f, 0.0f));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
            xCentro = xCentro + value_move;
        }
        else if (key == GLFW_KEY_LEFT) {
            matrix_translaction = glm::translate(matrix_translaction,
                                                     glm::vec3(-value_move, 0.0f, 0.0f));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
            xCentro = xCentro - value_move;
        }
        else if (key == GLFW_KEY_DOWN) {
            matrix_translaction = glm::translate(matrix_translaction,
                                                     glm::vec3(0.0f, value_move, 0.0f));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
            yCentro = yCentro + value_move;
        }
        else if (key == GLFW_KEY_UP) {
            matrix_translaction = glm::translate(matrix_translaction,
                                                     glm::vec3(0.0f, -value_move, 0.0f));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
            yCentro = yCentro - value_move;
        }
        else if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, true);
        }
        else if (key == GLFW_KEY_M)
        {
            matrix_scala = glm::scale(matrix_scala, glm::vec3(value_scala, value_scala, value_scala));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
        }
        else if (key == GLFW_KEY_L)
        {
            matrix_scala = glm::scale(matrix_scala, glm::vec3(1.0f / value_scala, 1.0f / value_scala, 1.0f / value_scala));
            matrix = matrix_translaction * matrix_rotation * matrix_scala;
        }
    }
}

void readData() {
	vector< vec3 > vertex;
	vector< vec3 > normals;
	vector< vec2 > mappings;

    ifstream arq("cube.obj");

    if (!arq){
        cout << "Archive not found!";
    } else {
        while (!arq.eof()) {
            string line;
            getline(arq, line);
            stringstream sline;
            sline << line;
            string temp;
            sline >> temp; // pega ate primeiro espaco
            if (temp == "v") {
                // ler vértice ...
                float x, y, z;
                sline >> x >> y >> z;
                // ... atribuir vértices da malha
                vec3 v = vec3(x, y, z);
                vertex.push_back( v );
            } else if (temp == "f") {
                // implementar lógica de varições
                // para face: v, v/t/n, v/t e v//n
                string token;
                sline >> token; // v/t/n, por exemplo
                stringstream stoken;
                stoken << token;
                string aux;
                getline(stoken, aux, '/');
                int x = stoi(aux)-1; // -1 pois o indice do arquivo é diferente do c++

                sline >> token; // v/t/n, por exemplo
                stoken.str("");// limpa a sstream para pegar apenas o primeiro
                stoken << token;
                getline(stoken, aux, '/');
                int y = stoi(aux)-1; // -1 pois o indice do arquivo é diferente do c++

                sline >> token; // v/t/n, por exemplo
                stoken.str("");// limpa a sstream para pegar apenas o primeiro
                stoken << token;
                getline(stoken, aux, '/');
                int z = stoi(aux)-1; // -1 pois o indice do arquivo é diferente do c++
                /* faces */
                vec3 v = vec3(x,y,z);
                Face *f = new Face(v);
                faces.push_back(f);

            }
        }

    }
	
	Group *g = new Group("cube", "stones", faces);
	groups.push_back( g );

	mesh = new Mesh(vertex, normals, mappings, groups);
}

int run() {
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
		640, 480, "Teste de versao OpenGL", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	// inicia manipulador da extens�o GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// obten��o de vers�o suportada da OpenGL e renderizador
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL (versao suportada) %s\n", version);
	// encerra contexto GL e outros recursos da GLFW

	/*
		Realiza a leitura dos dados para criar o Mesh
	*/
	readData();

    Group *g;
    vec3 v;
    Face *f;
    for (int i = 0; i < mesh->groups.size(); i++) {
        g = mesh->groups[i];

		vector<float> group_v;
		//vector<float*> vts;
		//vector<float*> vns;
		for (int j = 0; j < g->faces.size(); j++) {
		    f = g->faces[j];
			v = mesh->vertex[f->verts.x];
			group_v.push_back(v.x);
			group_v.push_back(v.y);
			group_v.push_back(v.z);

			v = mesh->vertex[f->verts.y];
			group_v.push_back(v.x);
			group_v.push_back(v.y);
			group_v.push_back(v.z);

			v = mesh->vertex[f->verts.z];
			group_v.push_back(v.x);
			group_v.push_back(v.y);
			group_v.push_back(v.z);

			/*
			vt = mesh->texts[f->texts[i]];
			vts.push_back(vt.x);
			vts.push_back(vt.y);
			vn = mesh->norms[f->norms[i]];
			vns.push_back(vn.x);
			vns.push_back(vn.y);
			vns.push_back(vn.z);
			*/

		}
		g->numVertices = group_v.size();
		printf("Num Vertices %d",g->numVertices);

		float*  arrayPoints = group_v.data();
		/*
		VBO
		*/
		glGenBuffers(1, &g->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, g->vbo);
		glBufferData(GL_ARRAY_BUFFER, g->numVertices * sizeof(float), arrayPoints, GL_STATIC_DRAW);

		/*
		VAO
		*/
		glGenVertexArrays(1, &g->vao);
		glBindVertexArray(g->vao);
		glBindBuffer(GL_ARRAY_BUFFER, g->vbo); // identifica vbo atual
											// habilitado primeiro atributo do vbo bound atual
		glEnableVertexAttribArray(0);
		// associa��o do vbo atual com primeiro atributo
		// 0 identifica que o primeiro atributo est� sendo definido
		// 3, GL_FLOAT identifica que dados s�o vec3 e est�o a cada 3 float.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		// � poss�vel associar outros atributos, como normais, mapeamento e cores
		// lembre-se: um por v�rtice
	}

	const char* vertex_shader =
		"#version 330\n"
		"layout(location=0) in vec3 vp;"
		"layout(location=1) in vec3 vc;"
		"uniform mat4 matrix;"
		"out vec3 color;"
		"void main () {"
		" color = vc;"
		" gl_Position = matrix * vec4(vp, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 330\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"void main () {"
		" frag_color = vec4 (1.0,1.0,1.0, 1.0);"
		"}";

	// identifica vs e o associa com vertex_shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	// identifica fs e o associa com fragment_shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	// identifica do programa, adiciona partes e faz "linkagem"
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	int matrixLocation = glGetUniformLocation(shader_programme, "matrix");


    glfwSetKeyCallback(window, key_callback);

    glClearColor(0.4f, 0.878f, 0.784f, 0.0f);

	glUseProgram(shader_programme);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Group *g;
		for (int i = 0; i < mesh->groups.size(); i++) {
			g = mesh->groups[i];
			// Define vao como verte array atual
			glBindVertexArray(g->vao);
			// desenha pontos a partir do p0 e 3 no total do VAO atual com o shader
			// atualmente em uso

			//pass uniform location to shader
			glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));

			glDrawArrays(GL_LINE_LOOP, 0, g->numVertices);
		}
		glfwSwapBuffers(window);

		/* para a janela 'responder' */
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}