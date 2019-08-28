#include "run.h"

vector<Face*> faces;
vector<Group*> groups;
Mesh *mesh;

void readData() {
	/*MESH*/
	/*
	float points[8][3] = {
		{0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, -0.5f},
		{0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},
		{-0.5f, 0.5f, 0.5f},
		{-0.5f, -0.5f, 0.5f},
		{-0.5f, 0.5f, -0.5f},
		{0.5f, -0.5f, 0.5f}
	};
	*/
	/*FACES*/
	/*
	int lado1[2][3] = {
		{2, 1, 7},
		{0,1,7}
	};
	int lado2[2][3] = {
		{4,0,5},
		{5,0,7}
	};
	int lado3[2][3] = {
		{0,6,4},
		{0,6,1}
	};
	int opostoLado2[2][3] = {
		{6,1,2},
		{6,3,2}
	};
	int opostoLado1[2][3] = {
		{3,5,4},
		{3,6,4}
	};
	int opostoLado3[2][3] = {
		{3,2,7},
		{3,7,5}
	};
	*/
	float points[8][3] = {
		{0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, -0.5f},
		{0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},
		{-0.5f, 0.5f, 0.5f},
		{-0.5f, -0.5f, 0.5f},
		{-0.5f, 0.5f, -0.5f},
		{0.5f, -0.5f, 0.5f}
	};
	int faceIndexes[12][3] = {
		{ 2, 1, 7},
		{ 0, 1, 7 },
		{ 4, 0, 5 },
		{ 5, 0, 7 },
		{ 0, 6, 4 },
		{ 0,6,1 },
		{ 6,1,2 },
		{ 6,3,2 },
		{ 3,5,4 },
		{ 3,6,4 },
		{ 3,2,7 },
		{ 3,7,5 }
	};

	vector< vec3 > vertex;
	vector< vec3 > normals;
	vector< vec2 > mappings;

	/* points */
	for (int i = 0; i < 8; i++) {
		vec3 v = vec3(points[i][0], points[i][1], points[i][2]);
		vertex.push_back( v );
	}
	/* faces */
	for (int i = 0; i < 12; i++) {
		vec3 v = vec3(faceIndexes[i][0], faceIndexes[i][1], faceIndexes[i][2]);
		Face *f = new Face(v);
		faces.push_back(f);
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
	/* Caso necessário, definições específicas para SOs, p. e. Apple OSX *
	Definir como 3.2 para Apple OS X */

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//*/


	GLFWwindow* window = glfwCreateWindow(
		640, 480, "Teste de versão OpenGL", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	// inicia manipulador da extensão GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// obtenção de versão suportada da OpenGL e renderizador
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL (versao suportada) %s\n", version);
	// encerra contexto GL e outros recursos da GLFW

	glm::mat4 matrix;
	glm::mat4 matrix_rotation = glm::mat4(1);
	matrix_rotation = glm::rotate(matrix_rotation, glm::radians(45.0f), glm::vec3(1, 0, 1));

	matrix = matrix_rotation;

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
		g->vertexSize = group_v.size();
		float*  arrayPoints = group_v.data();
		/*
		VBO
		*/
		glGenBuffers(1, &g->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, g->vbo);
		glBufferData(GL_ARRAY_BUFFER, group_v.size() * sizeof(float), arrayPoints, GL_STATIC_DRAW);

		/*
		VAO
		*/
		glGenVertexArrays(1, &g->vao);
		glBindVertexArray(g->vao);
		glBindBuffer(GL_ARRAY_BUFFER, g->vbo); // identifica vbo atual
											// habilitado primeiro atributo do vbo bound atual
		glEnableVertexAttribArray(0);
		// associação do vbo atual com primeiro atributo
		// 0 identifica que o primeiro atributo está sendo definido
		// 3, GL_FLOAT identifica que dados são vec3 e estão a cada 3 float.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		// é possível associar outros atributos, como normais, mapeamento e cores
		// lembre-se: um por vértice
	}

	const char* vertex_shader =
		"#version 330\n"
		"layout(location=0) in vec3 vp;"
		"layout(location=1) in vec3 vc;"
		"uniform mat4 matrix;"
		"out vec3 color;"
		"void main () {"
		" color = vc;"
		" gl_Position = matrix * vec4 (vp, 1.0);"
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

			glDrawArrays(GL_LINE_LOOP, 0, g->vertexSize);
		}
		glfwSwapBuffers(window);

		/* para a janela 'responder' */
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}