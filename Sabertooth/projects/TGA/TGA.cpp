#include "TGA.h"

#define WEIGTH 800
#define HEIGHT 600

#define N_ROTATE_DEGREE 10
#define PORCENT_SCALE   10
#define N_MOVE          0.1


bool keys[1024];

float xCentro = WEIGTH/2;
float yCentro = HEIGHT/2;
float value_move = 0.10f;
float angle_rotation = 1.0;
float cameraSpeed = 1.f; // adjust accordingly
float fov = 45.0f;

float pitchAngle = -30.f;
float yawAngle = -90.f;

float directionSpeed = 25.f;
int indexSelectedObject = -1;

vector<Face*> faces;
vector<Group*> groups;
vector<Material*> materials;
vector<Obj3d*> objs;

FileReader fileReader;

glm::mat4 proj = glm::perspective(glm::radians(fov),((float)WEIGTH)/((float)HEIGHT),0.1f,100.0f);

//eye: posição
glm::vec3 cameraPos = glm::vec3(0.f, 2.f, 4.f);
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

//Define acoes do redimensionamento da tela
void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 90.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 90.0f)
        fov = 90.0f;
}


bool testingIfKeysPressed(){
    const int size = 25;

    int relevantKeys[size] ={
            //camera keys
            GLFW_KEY_Q,
            GLFW_KEY_E,
            GLFW_KEY_RIGHT,
            GLFW_KEY_LEFT,
            GLFW_KEY_DOWN,
            GLFW_KEY_UP,

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
            GLFW_KEY_Z
    };

    for (int i = 0; i < size; ++i)
        if(keys[relevantKeys[i]]) return true;

    return false;


}

void keyboard_reaction(float elapsedTime, bool &hasKeyboardPressed) {

    hasKeyboardPressed =testingIfKeysPressed();

    //TESTA SE EXISTE ALGUM SHIFT PRESSIONADO
    bool shiftPressed = (keys[GLFW_KEY_LEFT_SHIFT]||keys[GLFW_KEY_RIGHT_SHIFT]);


    // USADO PARA SELECAO DE OBJETOS
    for(int i = GLFW_KEY_0; i <= GLFW_KEY_9; i++){
        if(keys[i])
            indexSelectedObject = i - GLFW_KEY_0 - 1;
    }

    //MOVIMENTOS DO OBJETO SELECIONADO
    if(indexSelectedObject>-1 && indexSelectedObject < objs.size()){

        bool shiftPressed = (keys[GLFW_KEY_LEFT_SHIFT]||keys[GLFW_KEY_RIGHT_SHIFT]);

        if (keys[GLFW_KEY_R]&&shiftPressed==false)
            objs[indexSelectedObject]->rotate(N_ROTATE_DEGREE);

        if (keys[GLFW_KEY_R]&&shiftPressed)
            objs[indexSelectedObject]->rotate(-N_ROTATE_DEGREE);

        if (keys[GLFW_KEY_S]&&shiftPressed==false)
            objs[indexSelectedObject]->scale(1+PORCENT_SCALE/100.f);

        if (keys[GLFW_KEY_S]&&shiftPressed)
            objs[indexSelectedObject]->scale(1-PORCENT_SCALE/100.f);

        if (keys[GLFW_KEY_X]&&shiftPressed==false)
            objs[indexSelectedObject]->move(N_MOVE,0,0);

        if (keys[GLFW_KEY_X]&&shiftPressed)
            objs[indexSelectedObject]->move(-N_MOVE,0,0);

        if (keys[GLFW_KEY_Y]&&shiftPressed==false)
            objs[indexSelectedObject]->move(0,N_MOVE,0);

        if (keys[GLFW_KEY_Y]&&shiftPressed)
            objs[indexSelectedObject]->move(0,-N_MOVE,0);

        if (keys[GLFW_KEY_Z]&&shiftPressed==false)
            objs[indexSelectedObject]->move(0,0,N_MOVE);

        if (keys[GLFW_KEY_Z]&&shiftPressed)
            objs[indexSelectedObject]->move(0,0,-N_MOVE);
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
    if (keys[GLFW_KEY_RIGHT] && shiftPressed) {
        yawAngle += directionSpeed * elapsedTime;

        cameraRight = cross(direction, cameraUp);
    }
    if (keys[GLFW_KEY_LEFT] && shiftPressed) {
        yawAngle -= directionSpeed * elapsedTime;

        cameraRight = cross(direction, cameraUp);
    }

    if (keys[GLFW_KEY_DOWN] && shiftPressed) {
        pitchAngle -= directionSpeed* elapsedTime;
        if (pitchAngle > 89.0f)
            pitchAngle = 89.0f;
        if (pitchAngle < -89.0f)
            pitchAngle = -89.0f;

//		cameraUp = cross(direction, cameraRight);
    }

    if (keys[GLFW_KEY_UP] && shiftPressed) {
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

void createObjects(){
    // Realiza a leitura dos dados para criar o Mesh (campo)
    Mesh* mesh = fileReader.readOBJ("projects/TGA/objs/paintball/", "cenaPaintball.obj", 1, materials);

    // indica como ler os vertices
    loadVertices(mesh);

    //adiciona no vetor de objetos
    objs.push_back(new Obj3d(mesh, 0, 0, 0));
    objs[0]->scale((1/mesh->distanceScale) * 5);

    // Realiza a leitura dos dados para criar o Mesh (mesa01)
    Mesh* mesh2 = fileReader.readOBJ("projects/TGA/objs/mesa/", "mesa01.obj", 2, materials);

    // indica como ler os vertices
    loadVertices(mesh2);

    //mesa 01 (esquerda mais perto)
    objs.push_back(new Obj3d(mesh2, -0.85f, 0, 1.1f));
    objs[1]->scale((1 / mesh2->distanceScale)/2);

    //mesa 02 (esquerda mais longe)
    objs.push_back(new Obj3d(mesh2, -0.65f, 0, -1.3f));
    objs[2]->scale((1 / mesh2->distanceScale)/2);

    //mesa 01 (direita mais perto)
    objs.push_back(new Obj3d(mesh2, 0.85f, 0, 1.2f));
    objs[3]->scale((1 / mesh2->distanceScale)/2);

    //mesa 02 (esquerda mais longe)
    objs.push_back(new Obj3d(mesh2, 1.1f, 0, -0.6f));
    objs[4]->scale((1 / mesh2->distanceScale)/2);

    // Realiza a leitura dos dados para criar o Mesh (caixa)
    Mesh* mesh3 = fileReader.readOBJ("projects/TGA/objs/box/", "Crate1.obj", 1, materials);

    // indica como ler os vertices
    loadVertices(mesh3);
    objs.push_back(new Obj3d(mesh3, 0.3f, 0.145f, 0.8f));
    objs[5]->scale((1 / mesh3->distanceScale)/2);

    objs.push_back(new Obj3d(mesh3, 0.27f, 0.36f, 0.8f));
    objs[6]->scale((1 / mesh3->distanceScale)/4);
    objs[6]->rotate(15.0f);

    //caixote bem externo
    objs.push_back(new Obj3d(mesh3, 0, 2.85f, 0));
    float objSevenDistanceScale = (1 / mesh3->distanceScale);
    objs[7]->scale(objSevenDistanceScale*30, objSevenDistanceScale*10,objSevenDistanceScale*30);
}

void createShaders(GLuint &shader_programme){
    // criacao dos shaders
    char vertex_shader[1024 * 256];
    char fragment_shader[1024 * 256];

    fileReader.convertFileInString("projects/TGA/shaders/core.vert", vertex_shader, 1024 * 256);
    fileReader.convertFileInString("projects/TGA/shaders/core.frag", fragment_shader, 1024 * 256);

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
    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fShader);
    glAttachShader(shader_programme, vShader);
    glLinkProgram(shader_programme);

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
}

void verifyOpenGL(){
    // obtencao de versao suportada da OpenGL e renderizador
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n\n", renderer);
    printf("OpenGL (versao suportada) %s\n", version);
    // encerra contexto GL e outros recursos da GLFW
}

int createGlfwWindow(GLFWwindow* &window){

    /* Caso necessario, definicoes especificas para SOs, p. e. Apple OSX *
    Definir como 3.2 para Apple OS X */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //cria a janela
    window = glfwCreateWindow(
            WEIGTH, HEIGHT, "Cenário do Grau A", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    return 0;
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    //cria a janela com GLFW
    GLFWwindow* window;
    createGlfwWindow(window);

    // inicia manipulador da extensao GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    //habilita profundidade
    glEnable(GL_DEPTH_TEST);

    //verifica a versao do Open GL
    verifyOpenGL();

    //criacao dos objetos
    createObjects();

    //cricao de shaders
    GLuint shader_programme;
    createShaders(shader_programme);
    glUseProgram(shader_programme);

    //criacao das locations
    int viewLocation = glGetUniformLocation(shader_programme, "view");
    int projLocation = glGetUniformLocation(shader_programme, "proj");
    int modelLocation = glGetUniformLocation(shader_programme, "model");

    //define calbacks
    glfwSetKeyCallback(window, key_callback);// teclas
    glfwSetScrollCallback(window, scroll_callback);// scrool do mouse
    glfwSetWindowSizeCallback(window, window_size_callback);// redimensionar a tela

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

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        keyboard_reaction(elapsed_seconds, hasKeyboardPressed);

        proj = glm::perspective(glm::radians(fov), ((float)WEIGTH) / ((float)HEIGHT), 0.1f, 100.0f);
        view = glm::lookAt(cameraPos, cameraPos + direction, cameraUp);

        //passa uniform para o shader
        if(hasKeyboardPressed)
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));


        glUseProgram(shader_programme);
        for (int o = 0; o < objs.size(); o++) {

            glUniform1i((glGetUniformLocation(shader_programme, "selected")), indexSelectedObject==o);


            Group *g;
            for (int i = 0; i < objs[o]->mesh->groups.size(); i++) {
                g = objs[o]->mesh->groups[i];
                if (g->numVertices == 0) continue;

                g->material->bind(shader_programme);

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