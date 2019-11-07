#include "CurvesEditor.h"

#define WIDTH 800
#define HEIGHT 600

//Atributos janela
int RESIZED_WIDTH = WIDTH;
int RESIZED_HEIGHT = HEIGHT;

bool keys[1024];

GLuint pointsVAO, centralCurveVAO, internalCurveVAO, externalCurveVAO;
GLuint pointsVBO, centralCurveVBO, internalCurveVBO, externalCurveVBO;

vector<float> controlPoints;
vector<float> controlPointsX;
vector<float> controlPointsY;
vector<float> controlPointsZ;

vector<float> centralCurvePoints;
vector<float> centralCurvePointsX;
vector<float> centralCurvePointsY;
vector<float> centralCurvePointsZ;

vector<float> internalCurvePoints;
vector<float> internalCurvePointsX;
vector<float> internalCurvePointsY;
vector<float> internalCurvePointsZ;

vector<float> externalCurvePoints;
vector<float> externalCurvePointsX;
vector<float> externalCurvePointsY;
vector<float> externalCurvePointsZ;

float zColor = 0.0f;

bool isCurveDrawn = false;

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

void calulateSPlineCurve() {
    centralCurvePoints.clear();
    centralCurvePointsX.clear();
    centralCurvePointsY.clear();
    centralCurvePointsZ.clear();

    float inc = 0.01f;

    int n = controlPointsX.size();

    for (int i = 0; i < controlPointsX.size(); i += 1) {
        for (float j = 0.f; j <= 1.f; j += inc) {
            float x = (((-1 * pow(j, 3) + 3 * pow(j, 2) - 3 * j + 1.f) * controlPointsX[i] +
                        (3.f * pow(j, 3) - 6.f * pow(j, 2) + 0.f * j + 4.f) * controlPointsX[(i + 1) % n] +
                        (-3.f * pow(j, 3) + 3.f * pow(j, 2) + 3.f * j + 1.f) * controlPointsX[(i + 2) % n] +
                        (1.f * pow(j, 3) + 0.f * pow(j, 2) + 0.f * j + 0.f) * controlPointsX[(i + 3) % n]) / 6.f);
            float y = (((-1 * pow(j, 3) + 3 * pow(j, 2) - 3.f * j + 1.f) * controlPointsY[i] +
                        (3.f * pow(j, 3) - 6.f * pow(j, 2) + 0.f * j + 4.f) * controlPointsY[(i + 1) % n] +
                        (-3.f * pow(j, 3) + 3.f * pow(j, 2) + 3.f * j + 1.f) * controlPointsY[(i + 2) % n] +
                        (1.f * pow(j, 3) + 0.f * pow(j, 2) + 0.f * j + 0.f) * controlPointsY[(i + 3) % n]) / 6.f);
            float z = (((-1 * pow(j, 3) + 3 * pow(j, 2) - 3.f * j + 1.f) * controlPointsZ[i] +
                        (3.f * pow(j, 3) - 6.f * pow(j, 2) + 0.f * j + 4.f) * controlPointsZ[(i + 1) % n] +
                        (-3.f * pow(j, 3) + 3.f * pow(j, 2) + 3.f * j + 1.f) * controlPointsZ[(i + 2) % n] +
                        (1.f * pow(j, 3) + 0.f * pow(j, 2) + 0.f * j + 0.f) * controlPointsZ[(i + 3) % n]) / 6.f);

            centralCurvePoints.push_back(x);
            centralCurvePoints.push_back(y);
            centralCurvePoints.push_back(z);

            centralCurvePointsX.push_back(x);
            centralCurvePointsY.push_back(y);
            centralCurvePointsZ.push_back(z);
        }
    }
}

void drawControlPoints(){
    glBindVertexArray(pointsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(float), controlPoints.data(), GL_STATIC_DRAW);
}

void drawCentralCurve() {
    glBindVertexArray(centralCurveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, centralCurveVBO);
    glBufferData(GL_ARRAY_BUFFER, centralCurvePoints.size() * sizeof(float), centralCurvePoints.data(), GL_STATIC_DRAW);
}

void drawInternalCurve() {
    glBindVertexArray(internalCurveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, internalCurveVBO);
    glBufferData(GL_ARRAY_BUFFER, internalCurvePoints.size() * sizeof(float), internalCurvePoints.data(), GL_STATIC_DRAW);
}

void drawExternalCurve() {
    glBindVertexArray(externalCurveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, externalCurveVBO);
    glBufferData(GL_ARRAY_BUFFER, externalCurvePoints.size() * sizeof(float), externalCurvePoints.data(), GL_STATIC_DRAW);
}

/*
Define acoes do mouse
*/
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            double xPos, yPos;

            glfwGetCursorPos(window, &xPos, &yPos);

            //Realiza a proporcao do clique para projecao original
            xPos = WIDTH * xPos / RESIZED_WIDTH;
            yPos = HEIGHT * yPos / RESIZED_HEIGHT;

            controlPoints.push_back(xPos);
            controlPoints.push_back(yPos);
            controlPoints.push_back(zColor);

            controlPointsX.push_back(xPos);
            controlPointsY.push_back(yPos);
            controlPointsZ.push_back(zColor);

            drawControlPoints();

            if (controlPointsX.size() >= 4) {

                calulateSPlineCurve();
                isCurveDrawn = true;

                drawCentralCurve();
            }

        }
    }
}


void keyboard_reaction(float elapsedTime, bool &hasKeyboardPressed) {
    float inc = 0.01f;
    float limite = 1.0f;

    if (keys[GLFW_KEY_PERIOD] == 1) {
        if (zColor < limite) {
            zColor = zColor + inc;
            controlPoints[controlPoints.size()-1] = zColor;
            drawControlPoints();

            calulateSPlineCurve();
            drawCentralCurve();

        }
    }

    if (keys[GLFW_KEY_COMMA] == 1) {
        if (zColor > 0.f){
            zColor = zColor - inc;
            controlPoints[controlPoints.size()-1] = zColor;
            drawControlPoints();

            calulateSPlineCurve();
            drawCentralCurve();
        }
    }


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

void initializeVertexBuffers() {

    // VAO e VBO to points
    glGenVertexArrays(1, &pointsVAO);
    glBindVertexArray(pointsVAO);

    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glEnableVertexAttribArray(0);// habilitado primeiro atributo do shader
    // associacao do vbo atual com primeiro atributo
    // 0 identifica que o primeiro atributo está sendo definido
    // 2, GL_FLOAT identifica que dados sao vec3 e estao a cada 2 float.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


    // VAO e VBO to centralCurve
    glGenVertexArrays(1, &centralCurveVAO);
    glBindVertexArray(centralCurveVAO);

    glGenBuffers(1, &centralCurveVBO);
    glBindBuffer(GL_ARRAY_BUFFER, centralCurveVBO);
    glEnableVertexAttribArray(0);// habilitado primeiro atributo do shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


    // VAO e VBO to centralCurve
    glGenVertexArrays(1, &centralCurveVAO);
    glBindVertexArray(centralCurveVAO);

    glGenBuffers(1, &centralCurveVBO);
    glBindBuffer(GL_ARRAY_BUFFER, centralCurveVBO);
    glEnableVertexAttribArray(0);// habilitado primeiro atributo do shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


    // VAO e VBO to internalCurve
    glGenVertexArrays(1, &internalCurveVAO);
    glBindVertexArray(internalCurveVAO);

    glGenBuffers(1, &internalCurveVBO);
    glBindBuffer(GL_ARRAY_BUFFER, internalCurveVBO);
    glEnableVertexAttribArray(0);// habilitado primeiro atributo do shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


    // VAO e VBO to externalCurve
    glGenVertexArrays(1, &externalCurveVAO);
    glBindVertexArray(externalCurveVAO);

    glGenBuffers(1, &externalCurveVBO);
    glBindBuffer(GL_ARRAY_BUFFER, externalCurveVBO);
    glEnableVertexAttribArray(0);// habilitado primeiro atributo do shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

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

    // cricao de shaders
    Shader *shader = new Shader("projects/CurvesEditor/shaders/core.vert", "projects/CurvesEditor/shaders/core.frag");
    shader->use();

    //inicializa VAOs e VBOs
    initializeVertexBuffers();

    // criacao das locations
    int projLocation = glGetUniformLocation(shader->shader_programme, "proj");

    // criacao glm projection
    glm::mat4 projection = glm::ortho(0.0f, (float) WIDTH, (float) HEIGHT, 0.0f, -1.0f, 1.0f);

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

        // desenha apenas os pontos
        glBindVertexArray(pointsVAO);
        glDrawArrays(GL_POINTS, 0, controlPointsX.size());

        // desenha a curva
        if (isCurveDrawn) {
            glBindVertexArray(centralCurveVAO);
            glDrawArrays(GL_LINE_STRIP, 0, centralCurvePointsX.size());
        }

        glfwSwapBuffers(window);

        /* para a janela 'responder' */
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}