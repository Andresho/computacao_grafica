#include "run.h"

#define WEIGTH 800
#define HEIGHT 600

vector<Face*> faces;
vector<Group*> groups;

glm::mat4 model_translaction = glm::mat4(1);
glm::mat4 model_rotation = glm::mat4(1);
glm::mat4 model_scala = glm::mat4(1);
glm::mat4 model = model_translaction * model_rotation * model_scala;

glm::mat4 proj = glm::perspective(glm::radians(45.0f),((float)WEIGTH)/((float)HEIGHT),0.1f,100.0f);
/*
//eye: posição
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

// direction: direção
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection =   glm::normalize(cameraPos - cameraTarget);

// right: direita. Vetor perpendicular ao plano direction-up
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight =  glm::normalize(glm::cross(up, cameraDirection));
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

//up: teto. Vetor perpendicular ao plano direction-right
glm::vec3 cameraUp =  glm::cross(cameraDirection, cameraRight);


//glm::mat4 view = //glm::lookAt(glm::vec3(0.0f, 0.0f, 0.5f),
                   //                glm::vec3(0.0f, 0.0f, 0.0f),
                     //        cameraUp);
//        glm::lookAt(cameraPos, cameraTarget, cameraUp);
//
*/
glm::mat4 view =glm::mat4(1);

float xCentro = WEIGTH/2;
float yCentro = HEIGHT/2;
float value_scala = 1.2f;
float value_move = 0.10f;
float angle_rotation = 1.0;
//Define acoes do teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if ((action == GLFW_REPEAT || action == GLFW_PRESS)) {
        if (key == GLFW_KEY_U) {
            model_rotation = glm::rotate(model_rotation, glm::radians(-angle_rotation), glm::vec3(1, 0, 0));
            model = model_translaction * model_rotation * model_scala;
        }
        else if (key == GLFW_KEY_R) {
            model_rotation = glm::rotate(model_rotation, glm::radians(angle_rotation), glm::vec3(1, 0, 0));
            model = model_translaction * model_rotation * model_scala;
        }

        if (key == GLFW_KEY_I) {
            model_rotation = glm::rotate(model_rotation, glm::radians(-angle_rotation), glm::vec3(0, 1, 0));
            model = model_translaction * model_rotation * model_scala;
        }
        else if (key == GLFW_KEY_E) {
            model_rotation = glm::rotate(model_rotation, glm::radians(angle_rotation), glm::vec3(0, 1, 0));
            model = model_translaction * model_rotation * model_scala;
        }

        if (key == GLFW_KEY_O) {
            model_rotation = glm::rotate(model_rotation, glm::radians(-angle_rotation), glm::vec3(0, 0, 1));
            model = model_translaction * model_rotation * model_scala;
        }
        else if (key == GLFW_KEY_W) {
            model_rotation = glm::rotate(model_rotation, glm::radians(angle_rotation), glm::vec3(0, 0, 1));
            model = model_translaction * model_rotation * model_scala;
        }

        else if (key == GLFW_KEY_RIGHT) {
            model_translaction = glm::translate(model_translaction,
                                                glm::vec3(value_move, 0.0f, 0.0f));
            model = model_translaction * model_rotation * model_scala;
            xCentro = xCentro + value_move;
        }
        else if (key == GLFW_KEY_LEFT) {
            model_translaction = glm::translate(model_translaction,
                                                glm::vec3(-value_move, 0.0f, 0.0f));
            model = model_translaction * model_rotation * model_scala;
            xCentro = xCentro - value_move;
        }
        else if (key == GLFW_KEY_DOWN) {
            model_translaction = glm::translate(model_translaction,
                                                glm::vec3(0.0f, -value_move, 0.0f));
            model = model_translaction * model_rotation * model_scala;
            yCentro = yCentro + value_move;
        }
        else if (key == GLFW_KEY_UP) {
            model_translaction = glm::translate(model_translaction,
                                                glm::vec3(0.0f, value_move, 0.0f));
            model = model_translaction * model_rotation * model_scala;
            yCentro = yCentro - value_move;
        }
        else if (key == GLFW_KEY_A) {
            model_translaction = glm::translate(model_translaction,
                                                glm::vec3(0.0f, 0.0f,value_move));
            model = model_translaction * model_rotation * model_scala;
        }
        else if (key == GLFW_KEY_S) {
            model_translaction = glm::translate(model_translaction,
                                                glm::vec3(0.0f, 0.0f,-value_move));
            model = model_translaction * model_rotation * model_scala;
        }
        else if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, true);
        }
        else if (key == GLFW_KEY_M)
        {
            model_scala = glm::scale(model_scala, glm::vec3(value_scala, value_scala, value_scala));
            model = model_translaction * model_rotation * model_scala;
        }
        else if (key == GLFW_KEY_L)
        {
            model_scala = glm::scale(model_scala, glm::vec3(1.0f / value_scala, 1.0f / value_scala, 1.0f / value_scala));
            model = model_translaction * model_rotation * model_scala;
        }
    }
}

Mesh* readData(string fileName) {
    Mesh *mesh = new Mesh();
    Group *g = nullptr;

    ifstream arq(fileName);

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
                // atribuir vértices da malha
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
            else if (temp == "g") {
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
                    for(int i=1; i<3; i++){
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
        printf("Num Vertices %d",g->numVertices);

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
    Mesh* mesh = readData("cube.obj");

    // indica como ler os vertices
    loadVertices(mesh);

    const char* vertex_shader =
            "#version 330\n"
            "layout(location=0) in vec3 vp;"
            "layout(location=1) in vec3 vn;"
            "layout(location=2) in vec2 vt;"
            "uniform mat4 view, proj, model;"
            "out vec3 color;"
            "void main () {"
            " color = vn;"
            " gl_Position = view * proj * model * vec4(vp, 1.0);"
            "}";

    const char* fragment_shader =
            "#version 330\n"
            "in vec3 color;"
            "out vec4 frag_color;"
            "void main () {"
            " frag_color = vec4 (color, 1.0);"
            "}";

    // identifica vShader e o associa com vertex_shader
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertex_shader, NULL);
    glCompileShader(vShader);
    // identifica fShader e o associa com fragment_shader
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragment_shader, NULL);
    glCompileShader(fShader);
    // identifica do programa, adiciona partes e faz "linkagem"
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fShader);
    glAttachShader(shader_programme, vShader);
    glLinkProgram(shader_programme);

    int viewLocation = glGetUniformLocation(shader_programme, "view");
    int projLocation = glGetUniformLocation(shader_programme, "proj");
    int modelLocation = glGetUniformLocation(shader_programme, "model");



    glfwSetKeyCallback(window, key_callback);

    glClearColor(0.4f, 0.278f, 0.184f, 0.0f);

    glUseProgram(shader_programme);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Group *g;
        for (int i = 0; i < mesh->groups.size(); i++) {
            g = mesh->groups[i];
            // Define vao como verte array atual
            glBindVertexArray(g->vao);

            //pass uniform location to shader
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));


            glDrawArrays(GL_LINE_LOOP, 0,g->numVertices);
//            glDrawArrays(GL_TRIANGLES, 0, g->numVertices);
        }
        glfwSwapBuffers(window);

        /* para a janela 'responder' */
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}