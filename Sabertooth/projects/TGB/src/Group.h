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

    ~Group() {};
};

