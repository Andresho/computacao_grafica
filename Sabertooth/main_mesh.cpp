#include "System.h"
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	/* Caso necessário, definições específicas para SOs, p. e. Apple OSX *
	/* Definir como 3.2 para Apple OS X */
	/*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/


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

	/*MESH*/
	float points[] = { 
		0.5f, 0.5f, 0.5f, 
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, 0.5f
	};

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
	int faces[12][3] = {
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

	//for (g in mesh->groups) {
	vector<float> vs;
	//vector<float*> vts;
	//vector<float*> vns;
	//for (int i = 0; i<faces. {
	for (i : 0 to f->numVertices) {
		v = mesh->verts[f->verts[i]];
		vs.push_back(v.x);
		vs.push_back(v.y);
		vs.push_back(v.z);
		vt = mesh->texts[f->texts[i]];
		vts.push_back(vt.x);
		vts.push_back(vt.y);
		vn = mesh->norms[f->norms[i]];
		vns.push_back(vn.x);
		vns.push_back(vn.y);
		vns.push_back(vn.z);
	}
	//}

	/*
	VBO
	*/
	float points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	float colors[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	GLuint colorsVBO = 0;
	glGenBuffers(1, &colorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors, GL_STATIC_DRAW);


	/*
	VAO
	*/
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // identifica vbo atual
										// habilitado primeiro atributo do vbo bound atual
	glEnableVertexAttribArray(0);
	// associação do vbo atual com primeiro atributo
	// 0 identifica que o primeiro atributo está sendo definido
	// 3, GL_FLOAT identifica que dados são vec3 e estão a cada 3 float.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// é possível associar outros atributos, como normais, mapeamento e cores
	// lembre-se: um por vértice!
	glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	// habilitado segundo atributo do vbo bound atual
	glEnableVertexAttribArray(1);
	// note que agora o atributo 1 está definido
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);



	const char* vertex_shader =
		"#version 330\n"
		"layout(location=0) in vec3 vp;"
		"layout(location=1) in vec3 vc;"
		"out vec3 color;"
		"void main () {"
		" color = vc;"
		" gl_Position = vec4 (vp, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 330\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"void main () {"
		" frag_color = vec4 (color, 1.0);"
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


	// passagem de variáveis CPU para GPU
	//GLint location = glGetUniformLocation(shader_programme, "inColor");
	//glUniform4f(location, 0.0f, 0.0f, 1.0f, 1.0f);



	glClearColor(0.4f, 0.878f, 0.784f, 0.0f);

	glUseProgram(shader_programme);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Define vao como verte array atual
		glBindVertexArray(vao);
		// desenha pontos a partir do p0 e 3 no total do VAO atual com o shader
		// atualmente em uso


		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);

		/* para a janela 'responder' */
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}
