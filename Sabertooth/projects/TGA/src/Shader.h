#pragma once
 class Shader
{
public:
    GLuint shader_programme;

    Shader(string vsPath,string fsPath){
        createShaders(vsPath,fsPath);
    }

    void use(){
        glUseProgram(this->shader_programme);
    }

     void createShaders(string vsPath,string fsPath){
         // criacao dos shaders
         char vertex_shader[1024 * 256];
         char fragment_shader[1024 * 256];

         FileReader fileReader;

         fileReader.convertFileInString(vsPath.c_str(), vertex_shader, 1024 * 256);
         fileReader.convertFileInString(fsPath.c_str(), fragment_shader, 1024 * 256);

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
         this->shader_programme = glCreateProgram();
         glAttachShader(this->shader_programme, fShader);
         glAttachShader(this->shader_programme, vShader);
         glLinkProgram(this->shader_programme);

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





 };


