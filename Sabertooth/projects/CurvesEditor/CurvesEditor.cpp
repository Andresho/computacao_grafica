#include "CurvesEditor.h"

#define WIDTH 800
#define HEIGHT 600

//Atributos janela
int RESIZED_WIDTH = WIDTH;
int RESIZED_HEIGHT = HEIGHT;

bool keys[1024];

GLuint vao;
vector<float> arrayPoints;

vector<Face *> faces;
vector<Group *> groups;
vector<Material *> materials;
vector<Obj3d *> objs;

FileReader fileReader;

//Define acoes do redimensionamento da tela
void window_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    RESIZED_WIDTH = width;
    RESIZED_HEIGHT = height;
}

/*
Define acoes do mouse
*/
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        double xPos, yPos;

        glfwGetCursorPos(window, &xPos, &yPos);

        //Realiza a proporcao do clique para projecao original
        xPos = WIDTH * xPos / RESIZED_WIDTH;
        yPos = HEIGHT * yPos / RESIZED_HEIGHT;

        arrayPoints.push_back(xPos);
        arrayPoints.push_back(yPos);

    }
}

void keyboard_reaction(float elapsedTime, bool &hasKeyboardPressed) {

}

//Define acoes do teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) keys[key] = true;
    if (action == GLFW_RELEASE) keys[key] = false;
}

void verifyOpenGL() {
    // obtencao de versao suportada da OpenGL e renderizador
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    printf("Renderer: %s\n\n", renderer);
    printf("OpenGL (versao suportada) %s\n", version);
    // encerra contexto GL e outros recursos da GLFW
}



int createGlfwWindow(GLFWwindow *&window) {

    /* Caso necessario, definicoes especificas para SOs, p. e. Apple OSX *
    Definir como 3.2 para Apple OS X */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //cria a janela
    window = glfwCreateWindow(
            WIDTH, HEIGHT, "Curves Editor", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    return 0;
}

void createVAO (){
    // VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void loadVBO(){
    /* VBO vertices */
    GLuint vboVerts;
    glGenBuffers(1, &vboVerts);
    glBindBuffer(GL_ARRAY_BUFFER, vboVerts);
    glBufferData(GL_ARRAY_BUFFER, arrayPoints.size() * sizeof(float), arrayPoints.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);// habilitado primeiro atributo do shader
    // associacao do vbo atual com primeiro atributo
    // 0 identifica que o primeiro atributo está sendo definido
    // 2, GL_FLOAT identifica que dados sao vec3 e estao a cada 2 float.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}


void initPoints(){
    //p1
    arrayPoints.push_back(WIDTH/2.f);
    arrayPoints.push_back(HEIGHT/2.f);

    //p2
    arrayPoints.push_back(300.f);
    arrayPoints.push_back(400.f);
}


int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    //cria a janela com GLFW
    GLFWwindow *window;
    createGlfwWindow(window);

    // inicia manipulador da extensao GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    //habilita profundidade
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //verifica a versao do Open GL
    verifyOpenGL();


    createVAO();

    initPoints();

    loadVBO();

    // cricao de shaders
    Shader *shader = new Shader("projects/CurvesEditor/shaders/core.vert", "projects/CurvesEditor/shaders/core.frag");
    shader->use();

    // criacao das locations
    int projLocation = glGetUniformLocation(shader->shader_programme, "proj");

    // criacao glm projection
    glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);

    // passa projection ao shader
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

    //define calbacks
    glfwSetKeyCallback(window, key_callback);// teclas
    glfwSetWindowSizeCallback(window, window_size_callback);// redimensionar a tela
    glfwSetMouseButtonCallback(window, mouse_button_callback);// esta para quando clicar com o mouse

    glClearColor(0.4f, 0.278f, 0.184f, 0.0f);

    double previous_seconds = glfwGetTime();
    bool hasKeyboardPressed = false;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        keyboard_reaction(elapsed_seconds, hasKeyboardPressed);

        shader->use();

        glBufferData(GL_ARRAY_BUFFER, arrayPoints.size() * sizeof(float), arrayPoints.data(), GL_STATIC_DRAW);

        // Define vao como verte array atual
        glBindVertexArray(vao);


        // define o tipo de desenho
        glDrawArrays(GL_LINE_STRIP, 0, arrayPoints.size()/2);

        glfwSwapBuffers(window);

        /* para a janela 'responder' */
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}