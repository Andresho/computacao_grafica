#include "TGB.h"

// para ler curva setar 1
// para cena do paintball com mesas setar 2
#define CENARIO_ID 1
#define DEBUG false

#define WEIGTH 800
#define HEIGHT 600

#define N_ROTATE_DEGREE 10
#define PORCENT_SCALE 10
#define N_MOVE 0.1

#define CAR_SPEED_MAIN 10

float CAR_SPEED = CAR_SPEED_MAIN;
float lastTheta;

bool keys[1024];
int indexSelectedObject = -1;
glm::vec3 ilumination_Ia_Id_Is = glm::vec3(1.f);

float cameraSpeed = 1.f;
float fov = 45.0f;
float pitchAngle = -30.f;
float yawAngle = -90.f;
float directionSpeed = 25.f;

vector<float> centralCurvePointsX;
vector<float> centralCurvePointsY;
vector<float> centralCurvePointsZ;

float trackScale = 0.f;
int carIndex = 0;

vector<Face *> faces;
vector<Group *> groups;
vector<Material *> materials;
vector<Obj3d *> objs;

FileReader fileReader;

//projecao
glm::mat4 proj = glm::perspective(glm::radians(fov), ((float)WEIGTH) / ((float)HEIGHT), 0.1f, 100.0f);

//eye: posição
glm::vec3 cameraPos = glm::vec3(0.f, 2.f, 4.f);

// direction: direção
glm::vec3 direction = glm::vec3(
    glm::normalize(
        glm::vec3(
            cos(glm::radians(pitchAngle)) * cos(glm::radians(yawAngle)),
            sin(glm::radians(pitchAngle)),
            cos(glm::radians(pitchAngle)) * sin(glm::radians(yawAngle)))));

glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// right: direita. Vetor perpendicular ao plano direction-up
glm::vec3 cameraRight = glm::normalize(glm::cross(direction, cameraUp));

//inicializa view
glm::mat4 view = glm::mat4(1);

//Define acoes do redimensionamento da tela
void window_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 90.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 90.0f)
        fov = 90.0f;
}

void moveCar()
{
    float x, y, z;
    int n, current, next;
    int inc = CAR_SPEED;

    n = centralCurvePointsX.size();
    current = carIndex % n;
    next = (carIndex + inc) % n;

    x = (centralCurvePointsX[next] - centralCurvePointsX[current]) * trackScale;
    y = (centralCurvePointsY[next] - centralCurvePointsY[current]) * trackScale;
    z = (centralCurvePointsZ[next] - centralCurvePointsZ[current]) * trackScale;

    objs[1]->move(x, y, z);
    carIndex = carIndex + inc;

    float theta = glm::atan(z, x);

    float theta2 = glm::atan(y, x);
    if (x <= 0)
    {
        theta2 -= 3.14f;
    }

    if(DEBUG)  printf("rad: %f x: %f y: %f\n", theta2, x, y);

    objs[1]->rotateYZ(-theta, theta2);
}

bool testingIfKeysPressed()
{
    const int size = 29;

    int relevantKeys[size] = {
        //camera keys
        GLFW_KEY_Q,
        GLFW_KEY_E,
        GLFW_KEY_RIGHT,
        GLFW_KEY_LEFT,
        GLFW_KEY_DOWN,
        GLFW_KEY_UP,

        //para transladar carro
        GLFW_KEY_SPACE,

        //select objs keys
        GLFW_KEY_0,
        GLFW_KEY_1,
        GLFW_KEY_2,
        GLFW_KEY_3,
        GLFW_KEY_4,
        GLFW_KEY_5,
        GLFW_KEY_6,
        GLFW_KEY_7,
        GLFW_KEY_8,
        GLFW_KEY_9,

        //shift keys
        GLFW_KEY_LEFT_SHIFT,
        GLFW_KEY_RIGHT_SHIFT,

        //move object keys
        GLFW_KEY_R,
        GLFW_KEY_S,
        GLFW_KEY_X,
        GLFW_KEY_Y,
        GLFW_KEY_Z,

        //ilumination control
        GLFW_KEY_F1,
        GLFW_KEY_F2,
        GLFW_KEY_F3};

    for (int i = 0; i < size; ++i)
        if (keys[relevantKeys[i]])
            return true;

    return false;
}

void keyboard_reaction(float elapsedTime, bool &hasKeyboardPressed)
{

    hasKeyboardPressed = testingIfKeysPressed();

    //TESTA SE EXISTE ALGUM SHIFT PRESSIONADO
    bool shiftPressed = (keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT]);

    // USADO PARA SELECAO DE OBJETOS
    for (int i = GLFW_KEY_0; i <= GLFW_KEY_9; i++)
    {
        if (keys[i])
            indexSelectedObject = i - GLFW_KEY_0 - 1;
    }

    //MOVIMENTOS DO OBJETO SELECIONADO
    if (indexSelectedObject > -1 && indexSelectedObject < objs.size())
    {

        bool shiftPressed = (keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT]);

        if (keys[GLFW_KEY_R] && shiftPressed == false)
            objs[indexSelectedObject]->rotate(N_ROTATE_DEGREE);

        if (keys[GLFW_KEY_R] && shiftPressed)
            objs[indexSelectedObject]->rotate(-N_ROTATE_DEGREE);

        if (keys[GLFW_KEY_S] && shiftPressed == false)
            objs[indexSelectedObject]->scale(1 + PORCENT_SCALE / 100.f);

        if (keys[GLFW_KEY_S] && shiftPressed)
            objs[indexSelectedObject]->scale(1 - PORCENT_SCALE / 100.f);

        if (keys[GLFW_KEY_X] && shiftPressed == false)
            objs[indexSelectedObject]->move(N_MOVE, 0, 0);

        if (keys[GLFW_KEY_X] && shiftPressed)
            objs[indexSelectedObject]->move(-N_MOVE, 0, 0);

        if (keys[GLFW_KEY_Y] && shiftPressed == false)
            objs[indexSelectedObject]->move(0, N_MOVE, 0);

        if (keys[GLFW_KEY_Y] && shiftPressed)
            objs[indexSelectedObject]->move(0, -N_MOVE, 0);

        if (keys[GLFW_KEY_Z] && shiftPressed == false)
            objs[indexSelectedObject]->move(0, 0, N_MOVE);

        if (keys[GLFW_KEY_Z] && shiftPressed)
            objs[indexSelectedObject]->move(0, 0, -N_MOVE);
    }

    // MECHE A POSICAO DA CAMERA
    if (keys[GLFW_KEY_UP] && shiftPressed == false)
        cameraPos += cameraSpeed * direction * elapsedTime;
    if (keys[GLFW_KEY_DOWN] && shiftPressed == false)
        cameraPos -= cameraSpeed * direction * elapsedTime;
    if (keys[GLFW_KEY_LEFT] && shiftPressed == false)
        cameraPos -= cameraRight * cameraSpeed * elapsedTime;
    if (keys[GLFW_KEY_RIGHT] && shiftPressed == false)
        cameraPos += cameraRight * cameraSpeed * elapsedTime;
    if (keys[GLFW_KEY_Q])
        cameraPos -= cameraUp * cameraSpeed * elapsedTime;
    if (keys[GLFW_KEY_E])
        cameraPos += cameraUp * cameraSpeed * elapsedTime;

    //meche a direcao da camera
    if (keys[GLFW_KEY_RIGHT] && shiftPressed)
    {
        yawAngle += directionSpeed * elapsedTime;

        cameraRight = cross(direction, cameraUp);
    }
    if (keys[GLFW_KEY_LEFT] && shiftPressed)
    {
        yawAngle -= directionSpeed * elapsedTime;

        cameraRight = cross(direction, cameraUp);
    }

    if (keys[GLFW_KEY_DOWN] && shiftPressed)
    {
        pitchAngle -= directionSpeed * elapsedTime;
        if (pitchAngle > 89.0f)
            pitchAngle = 89.0f;
        if (pitchAngle < -89.0f)
            pitchAngle = -89.0f;
    }

    if (keys[GLFW_KEY_UP] && shiftPressed)
    {
        pitchAngle += directionSpeed * elapsedTime;
        if (pitchAngle > 89.0f)
            pitchAngle = 89.0f;
        if (pitchAngle < -89.0f)
            pitchAngle = -89.0f;
    }
    glm::vec3 front;
    front.x = cos(glm::radians(pitchAngle)) * cos(glm::radians(yawAngle));
    front.y = sin(glm::radians(pitchAngle));
    front.z = cos(glm::radians(pitchAngle)) * sin(glm::radians(yawAngle));
    direction = glm::normalize(front);

    // liga a iluminacao
    if (keys[GLFW_KEY_F1] && !shiftPressed)
        ilumination_Ia_Id_Is.x = 1.f;
    if (keys[GLFW_KEY_F2] && !shiftPressed)
        ilumination_Ia_Id_Is.y = 1.f;
    if (keys[GLFW_KEY_F3] && !shiftPressed)
        ilumination_Ia_Id_Is.z = 1.f;

    // desliga a iluminacao
    if (keys[GLFW_KEY_F1] && shiftPressed)
        ilumination_Ia_Id_Is.x = 0.f;
    if (keys[GLFW_KEY_F2] && shiftPressed)
        ilumination_Ia_Id_Is.y = 0.f;
    if (keys[GLFW_KEY_F3] && shiftPressed)
        ilumination_Ia_Id_Is.z = 0.f;

    float boost = 2.f;
    if(keys[GLFW_KEY_SPACE]){
        if(CAR_SPEED<CAR_SPEED_MAIN*boost)
            CAR_SPEED=CAR_SPEED_MAIN*boost;
    } else{
        CAR_SPEED=CAR_SPEED_MAIN;
    }


}

//Define acoes do teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        keys[key] = true;
    if (action == GLFW_RELEASE)
        keys[key] = false;
}

void createObjects()
{
    if (CENARIO_ID == 1)
    {
        // Realiza a leitura dos dados para criar o Mesh (campo)
        Mesh *mesh = fileReader.readOBJ("projects/CurvesEditor/export/", "pista.obj", 1, materials);

        // indica como ler os vertices
        mesh->loadVertices();

        //adiciona no vetor de objetos
        objs.push_back(new Obj3d(mesh, 0, 0, 0));
        trackScale = (1 / mesh->distanceScale) * 5;
        objs[0]->scale(trackScale);

        // Realiza a leitura dos dados para criar o Mesh (carro)
        Mesh *mesh2 = fileReader.readOBJ("projects/TGB/objs/car/", "car.obj", 2, materials);

        // indica como ler os vertices
        mesh2->loadVertices();

        objs.push_back(new Obj3d(mesh2,
                                 centralCurvePointsX[0] * trackScale,
                                 centralCurvePointsY[0] * trackScale,
                                 centralCurvePointsZ[0] * trackScale));
        objs[1]->scale((1 / mesh2->distanceScale) / 4);
    }
    else
    {

        // Realiza a leitura dos dados para criar o Mesh (campo)
        Mesh *mesh = fileReader.readOBJ("projects/TGB/objs/paintball/", "cenaPaintball.obj", 1, materials);

        // indica como ler os vertices
        mesh->loadVertices();

        //adiciona no vetor de objetos
        objs.push_back(new Obj3d(mesh, 0, 0, 0));
        objs[0]->scale((1 / mesh->distanceScale) * 5);

        // Realiza a leitura dos dados para criar o Mesh (mesa01)
        Mesh *mesh2 = fileReader.readOBJ("projects/TGB/objs/mesa/", "mesa01.obj", 2, materials);

        // indica como ler os vertices
        mesh2->loadVertices();

        objs.push_back(new Obj3d(mesh2, -0.3f, 0, 1.1f));
        objs[1]->scale((1 / mesh2->distanceScale) / 2.0f);

        //mesa 02 (esquerda mais longe)
        objs.push_back(new Obj3d(mesh2, -0.65f, 0, -1.3f));
        objs[2]->scale((1 / mesh2->distanceScale) / 2);

        //mesa 01 (direita mais perto)
        objs.push_back(new Obj3d(mesh2, 0.85f, 0, 1.2f));
        objs[3]->scale((1 / mesh2->distanceScale) / 2);

        //mesa 02 (esquerda mais longe)
        objs.push_back(new Obj3d(mesh2, 1.1f, 0, -0.6f));
        objs[4]->scale((1 / mesh2->distanceScale) / 2);
    }
}

void verifyOpenGL()
{
    // obtencao de versao suportada da OpenGL e renderizador
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    printf("Renderer: %s\n\n", renderer);
    printf("OpenGL (versao suportada) %s\n", version);
    // encerra contexto GL e outros recursos da GLFW
}

int createGlfwWindow(GLFWwindow *&window)
{

    /* Caso necessario, definicoes especificas para SOs, p. e. Apple OSX *
    Definir como 3.2 para Apple OS X */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //cria a janela
    window = glfwCreateWindow(
        WEIGTH, HEIGHT, "Cenário do Grau B", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    return 0;
}

int main()
{
    if (!glfwInit())
    {
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

    //verifica a versao do Open GL
    verifyOpenGL();

    //carrega caminho da pista
    fileReader.readCarPath(
        "projects/CurvesEditor/export/", "carPath.txt",
        centralCurvePointsX,
        centralCurvePointsY,
        centralCurvePointsZ);

    //criacao dos objetos
    createObjects();

    //cricao de shaders
    Shader *shader = new Shader("projects/TGB/shaders/core.vert", "projects/TGB/shaders/core.frag");
    shader->use();

    //criacao das locations
    int viewLocation = glGetUniformLocation(shader->shader_programme, "view");
    int projLocation = glGetUniformLocation(shader->shader_programme, "proj");
    int modelLocation = glGetUniformLocation(shader->shader_programme, "model");

    //define calbacks
    glfwSetKeyCallback(window, key_callback);                // teclas
    glfwSetScrollCallback(window, scroll_callback);          // scrool do mouse
    glfwSetWindowSizeCallback(window, window_size_callback); // redimensionar a tela

    //inicializa camera
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

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        keyboard_reaction(elapsed_seconds, hasKeyboardPressed);

        //move o carro
        moveCar();

        proj = glm::perspective(glm::radians(fov), ((float)WEIGTH) / ((float)HEIGHT), 0.1f, 100.0f);
        view = glm::lookAt(cameraPos, cameraPos + direction, cameraUp);

        //passa uniform para o shader
        if (hasKeyboardPressed)
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

        shader->use();

        glUniform3f(glGetUniformLocation(shader->shader_programme, "ilumination_Ia_Id_Is"),
                    ilumination_Ia_Id_Is.x, ilumination_Ia_Id_Is.y, ilumination_Ia_Id_Is.z);

        for (int o = 0; o < objs.size(); o++)
        {

            glUniform1i((glGetUniformLocation(shader->shader_programme, "selected")), indexSelectedObject == o);

            Group *g;
            for (int i = 0; i < objs[o]->mesh->groups.size(); i++)
            {
                g = objs[o]->mesh->groups[i];
                if (g->numVertices == 0)
                    continue;

                //passa a textura para o shader
                g->material->bind(shader->shader_programme);

                // passa os parametros do material para o shader( Kd, Ks, Ka e Ns)
                g->sendToShaderMaterialParams(shader->shader_programme);

                // Define vao como verte array atual
                glBindVertexArray(g->vao);

                //passa uniform para o shader
                glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(objs[o]->transformations));

                // define o tipo de desenho
                glDrawArrays(GL_TRIANGLES, 0, g->numVertices);
            }
        }

        glfwSwapBuffers(window);

        /* para a janela 'responder' */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}