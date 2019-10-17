#pragma once

class Group {
public:
    Material *material;
    string name;
    string materialName;

    GLuint vao;
    int numVertices = 0;

    vector<Face *> faces;

    Group(string name, string materialName) {
        this->name = name;
        this->materialName = materialName;
    };

    void sendToShaderMaterialParams(GLuint shaderProgram){
        glUniform3f(glGetUniformLocation(shaderProgram, "Ka"),
                    this->material->Ka->r,  this->material->Ka->g,  this->material->Ka->b);
        glUniform3f(glGetUniformLocation(shaderProgram, "Kd"),
                    this->material->Kd->r,  this->material->Kd->g,  this->material->Kd->b);
        glUniform3f(glGetUniformLocation(shaderProgram, "Ks"),
                    this->material->Ks->r,  this->material->Ks->g,  this->material->Ks->b);
        glUniform1f(glGetUniformLocation(shaderProgram, "Ns"),
                    this->material->Ns);
    }

    ~Group() {};
};

