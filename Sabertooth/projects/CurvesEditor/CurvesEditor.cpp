#include "CurvesEditor.h"

#define WIDTH 800
#define HEIGHT 600
#define CURVE_SIZE 20.0

//Atributos janela
int RESIZED_WIDTH = WIDTH;
int RESIZED_HEIGHT = HEIGHT;

bool keys[1024];

GLuint pointsVAO, centralCurveVAO, internalCurveVAO, externalCurveVAO;
GLuint pointsVBO, centralCurveVBO, internalCurveVBO, externalCurveVBO;

vector<float> controlPoints, controlPointsX, controlPointsY, controlPointsZ;
vector<float> centralCurvePoints, centralCurvePointsX, centralCurvePointsY, centralCurvePointsZ;
vector<float> internalCurvePoints, internalCurvePointsX, internalCurvePointsY, internalCurvePointsZ;
vector<float> externalCurvePoints, externalCurvePointsX, externalCurvePointsY, externalCurvePointsZ;

vector<glm::vec3 *> normals;

float zColor = 0.0f;

bool isCurveDrawn = false;

vector<Face *> faces;
vector<Group *> groups;
vector<Material *> materials;
vector<Obj3d *> objs;

FileReader fileReader;
FileWriter fileWriter;

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

    float inc = 0.001f;

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

void calculateAroundCurves() {

    internalCurvePoints.clear();
    internalCurvePointsX.clear();
    internalCurvePointsY.clear();
    internalCurvePointsZ.clear();

    externalCurvePoints.clear();
    externalCurvePointsX.clear();
    externalCurvePointsY.clear();
    externalCurvePointsZ.clear();

    float xVector, yVector, theta, perpedicAngle;

    for (int i = 0; i < centralCurvePointsX.size() - 1; i += 1) {

        xVector = centralCurvePointsX[i + 1] - centralCurvePointsX[i];
        yVector = centralCurvePointsY[i + 1] - centralCurvePointsY[i];
        theta = atan(yVector / xVector);

        //calcula a curval interna
        if (xVector < 0)
            perpedicAngle = theta - (M_PI / 2);
        else
            perpedicAngle = theta + (M_PI / 2);

        //seta parametros da curva interna
        internalCurvePoints.push_back((float) cos(perpedicAngle) * CURVE_SIZE + centralCurvePointsX[i]);
        internalCurvePoints.push_back((float) sin(perpedicAngle) * CURVE_SIZE + centralCurvePointsY[i]);
        internalCurvePoints.push_back((float) centralCurvePointsZ[i]);
        internalCurvePointsX.push_back((float) cos(perpedicAngle) * CURVE_SIZE + centralCurvePointsX[i]);
        internalCurvePointsY.push_back((float) sin(perpedicAngle) * CURVE_SIZE + centralCurvePointsY[i]);
        internalCurvePointsZ.push_back((float) centralCurvePointsZ[i]);

        //calcula a curval externa
        if (xVector < 0)
            perpedicAngle = theta + (M_PI / 2);
        else
            perpedicAngle = theta - (M_PI / 2);

        //seta parametros da curva externa
        externalCurvePoints.push_back((float) cos(perpedicAngle) * CURVE_SIZE + centralCurvePointsX[i]);
        externalCurvePoints.push_back((float) sin(perpedicAngle) * CURVE_SIZE + centralCurvePointsY[i]);
        externalCurvePoints.push_back((float) centralCurvePointsZ[i]);
        externalCurvePointsX.push_back((float) cos(perpedicAngle) * CURVE_SIZE + centralCurvePointsX[i]);
        externalCurvePointsY.push_back((float) sin(perpedicAngle) * CURVE_SIZE + centralCurvePointsY[i]);
        externalCurvePointsZ.push_back((float) centralCurvePointsZ[i]);

    }
}


void calculateNormals() {

    // limpara o vector
    normals.clear();

    float Ux, Uy, Uz, Vx, Vy, Vz;
    float Nx, Ny, Nz;

    /*
     * https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
     * A surface normal for a triangle can be calculated by taking the vector cross product of two edges of that triangle.
     * The order of the vertices used in the calculation will affect the direction of the normal (in or out of the face w.r.t. winding).
     * So for a triangle p1, p2, p3,
     * if the vector U = p2 - p1 and the vector V = p3 - p1 then the normal N = U X V and can be calculated by:

        Nx = UyVz - UzVy

        Ny = UzVx - UxVz

        Nz = UxVy - UyVx
     * */

    for (int i = 0; i < internalCurvePointsX.size() - 1; i += 1) {

        if (i % 2 != 0) {
            Ux = externalCurvePointsX[i] - internalCurvePointsX[i];
            Uy = externalCurvePointsY[i] - internalCurvePointsY[i];
            Uz = externalCurvePointsZ[i] - internalCurvePointsZ[i];
            Vx = externalCurvePointsX[i + 1] - internalCurvePointsX[i];
            Vy = externalCurvePointsY[i + 1] - internalCurvePointsY[i];
            Vz = externalCurvePointsZ[i + 1] - internalCurvePointsZ[i];
        } else {
            Ux = internalCurvePointsX[i + 1] - externalCurvePointsX[i + 1];
            Uy = internalCurvePointsY[i + 1] - externalCurvePointsY[i + 1];
            Uz = internalCurvePointsZ[i + 1] - externalCurvePointsZ[i + 1];
            Vx = internalCurvePointsX[i] - externalCurvePointsX[i + 1];
            Vy = internalCurvePointsY[i] - externalCurvePointsY[i + 1];
            Vz = internalCurvePointsZ[i] - externalCurvePointsZ[i + 1];
        }

        Nx = Uy * Vz - Uz * Vy;
        Ny = Uz * Vx - Ux * Vz;
        Nz = Ux * Vy - Uy * Vx;

        normals.push_back(new glm::vec3(Nx, Ny, Nz));
    }
}

void drawControlPoints() {
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
    glBufferData(GL_ARRAY_BUFFER, internalCurvePoints.size() * sizeof(float), internalCurvePoints.data(),
                 GL_STATIC_DRAW);
}

void drawExternalCurve() {
    glBindVertexArray(externalCurveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, externalCurveVBO);
    glBufferData(GL_ARRAY_BUFFER, externalCurvePoints.size() * sizeof(float), externalCurvePoints.data(),
                 GL_STATIC_DRAW);
}

void drawCurves() {
    drawCentralCurve();
    drawInternalCurve();
    drawExternalCurve();
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
                isCurveDrawn = true;

                calulateSPlineCurve();
                calculateAroundCurves();

                drawCurves();
            }

        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            calculateNormals();
            for (int i = 0; i < normals.size() - 1; i++) {
                printf("pos %f x %f  y %f  z %f \n", (float) i, normals[i]->x, normals[i]->y, normals[i]->z);
            }
            fileWriter.writeArchivesOBJandMTL(
                    internalCurvePointsX, internalCurvePointsY, internalCurvePointsZ,
                    externalCurvePointsX, externalCurvePointsY, externalCurvePointsZ,
                    normals
            );

            fileWriter.writeCarPath(
                    centralCurvePointsX,
                    centralCurvePointsY,
                    centralCurvePointsZ
            );

        }
    }
}


void keyboard_reaction(float elapsedTime, bool &hasKeyboardPressed) {
    float inc = 0.01f;
    float limite = 1.0f;

    if (keys[GLFW_KEY_PERIOD] == 1) {
        if (zColor < (limite - inc)) {
            zColor = zColor + inc;
            controlPoints[controlPoints.size() - 1] = zColor;
            drawControlPoints();

            controlPointsZ[controlPointsZ.size() - 1] = zColor;

            if (controlPointsX.size() >= 4) {
                calulateSPlineCurve();
                calculateAroundCurves();
                drawCurves();
            }

        }
    }

    if (keys[GLFW_KEY_COMMA] == 1) {
        if (zColor > (0.f + inc)) {
            zColor = zColor - inc;
            controlPoints[controlPoints.size() - 1] = zColor;
            drawControlPoints();

            controlPointsZ[controlPointsZ.size() - 1] = zColor;

            if (controlPointsX.size() >= 4) {
                calulateSPlineCurve();
                calculateAroundCurves();
                drawCurves();
            }
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

            glBindVertexArray(internalCurveVAO);
            glDrawArrays(GL_LINE_STRIP, 0, internalCurvePointsX.size());

            glBindVertexArray(externalCurveVAO);
            glDrawArrays(GL_LINE_STRIP, 0, externalCurvePointsX.size());

        }

        glfwSwapBuffers(window);

        /* para a janela 'responder' */
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}