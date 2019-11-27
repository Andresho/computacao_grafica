#pragma once

class FileReader {
public:
    FileReader() {

    }

    void readCarPath(
            string pathName,
            string fileName,
            vector<float> &centralCurvePointsX,
            vector<float> &centralCurvePointsY,
            vector<float> &centralCurvePointsZ
    ) {


        ifstream arq(pathName + fileName);
        if (!arq) {
            cout << "Archive not found!" + fileName;
        } else {
            while (!arq.eof()) {

                string line;
                getline(arq, line);
                stringstream sline;
                sline << line;
                string temp;
                sline >> temp; // pega ate primeiro espaco

                if (temp == "xyz") {
                    float x, y, z;
                    sline >> x >> y >> z;
                    centralCurvePointsX.push_back(x);
                    centralCurvePointsY.push_back(y);
                    centralCurvePointsZ.push_back(z);
                }

            }
        }
    }

    bool convertFileInString(const char *fileName, char *shaderStr, int maxLength) {
        shaderStr[0] = '\0'; // zera a string
        FILE *file = fopen(fileName, "r");
        if (!file) {
            return false;
        }
        int currentLen = 0;
        char line[2048];
        strcpy(line, ""); // limpa a linha
        while (!feof(file)) {
            if (nullptr != fgets(line, 2048, file)) {
                currentLen += strlen(line);
                strcat(shaderStr, line);
            }
        }
        if (EOF == fclose(file)) {
            return false;
        }
        return true;
    }

    void readMaterial(string pathName, string fileName, vector<Material *> &materials) {
        Material *material;

        ifstream arq(pathName + fileName);
        if (!arq) {
            cout << "Archive not found!" + fileName;
        } else {
            while (!arq.eof()) {

                string line;
                getline(arq, line);
                stringstream sline;
                sline << line;
                string temp;
                sline >> temp; // pega ate primeiro espaco

                if (temp == "newmtl") {
                    string nameMTL;
                    sline >> nameMTL;
                    material = new Material();
                    material->materialName = nameMTL;
                    materials.push_back(material);
                } else if (temp == "map_Kd") {
                    string nameTexture;
                    sline >> nameTexture;
                    string pathWithName = pathName + nameTexture;
                    materials[materials.size() - 1]->createTexture(pathWithName.c_str(), false);
                } else if (temp == "Kd") {
                    float x, y, z;
                    sline >> x >> y >> z;
                    vec3 *Kd = new vec3(x, y, z);
                    materials[materials.size() - 1]->Kd = Kd;
                } else if (temp == "Ka") {
                    float x, y, z;
                    sline >> x >> y >> z;
                    vec3 *Ka = new vec3(x, y, z);
                    materials[materials.size() - 1]->Ka = Ka;
                } else if (temp == "Ks") {
                    float x, y, z;
                    sline >> x >> y >> z;
                    vec3 *Ks = new vec3(x, y, z);
                    materials[materials.size() - 1]->Ks = Ks;
                } else if (temp == "Ns") {
                    float x;
                    sline >> x;
                    materials[materials.size() - 1]->Ns = (float) x;
                }


            }

        }
    }

    // discartPointId usado para ajusta o tipo de triangulacao
    Mesh *readOBJ(string pathName, string fileName, int discartPointId, vector<Material *> &materials) {
        Mesh *mesh = new Mesh();
        Group *g = nullptr;

        string materialName = "default";

        vec3 maxBoundBox = vec3(0.f, 0.f, 0.f);
        vec3 minBoundBox = vec3(0.f, 0.f, 0.f);

        ifstream arq(pathName + fileName);

        if (!arq) {
            cout << "Archive not found!" + fileName;
        } else {
            while (!arq.eof()) {
                string line;
                getline(arq, line);
                stringstream sline;
                sline << line;
                string temp;
                sline >> temp; // pega ate primeiro espaco
                if (temp == "usemtl") {

                    if (g == nullptr) {
                        g = new Group("default", materialName);
                    }
                    string mtName;
                    sline >> mtName;
                    materialName = mtName;
                    g->materialName = mtName;
                } else if (temp == "mtllib") {
                    string fName;
                    sline >> fName;
                    readMaterial(pathName, fName, materials);
                } else if (temp == "v") {
                    // ler vértice ...
                    float x, y, z;
                    sline >> x >> y >> z;
                    // atribuir vértices da malha
                    if (x > maxBoundBox.x)
                        maxBoundBox.x = x;
                    if (y > maxBoundBox.y)
                        maxBoundBox.y = y;
                    if (z > maxBoundBox.z)
                        maxBoundBox.z = z;

                    if (x < minBoundBox.x)
                        minBoundBox.x = x;
                    if (y < minBoundBox.y)
                        minBoundBox.y = y;
                    if (z < minBoundBox.z)
                        minBoundBox.z = z;
                    vec3 *v = new vec3(x, y, z);
                    mesh->vertex.push_back(v);
                } else if (temp == "vn") {
                    // ler normais ...
                    float x, y, z;
                    sline >> x >> y >> z;
                    //atribuir normais da malha
                    vec3 *vn = new vec3(x, y, z);
                    mesh->normals.push_back(vn);
                } else if (temp == "vt") {
                    // ler mapeamento de textura ...
                    float x, y;
                    sline >> x >> y;
                    // atribuir mapeamentos de textura da malha
                    vec2 *vt = new vec2(x, y);
                    mesh->mappings.push_back(vt);
                } else if (temp == "g" || temp == "s") {
                    if (g != nullptr) {
                        mesh->groups.push_back(g);
                    }
                    string gName;
                    sline >> gName;
                    g = new Group(gName, materialName);
                } else if (temp == "f") {
                    //se grupo nao existe, cria um padrao
                    if (g == nullptr) {
                        g = new Group("default", materialName);
                    }
                    // implementar lógica de varições
                    // para face: v, v/t/n, v/t e v//n
                    Face *f = new Face();
                    Face *fAux = new Face();
                    int vertices = 0;

                    // le todas faces ate final da linha
                    while (!sline.eof()) {
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
                                        if (vertices > 3)
                                            fAux->verts.push_back(-1);
                                        else
                                            f->verts.push_back(-1);
                                    } else {
                                        if (vertices > 3)
                                            fAux->verts.push_back(stoi(aux[i]) - 1);
                                        else
                                            f->verts.push_back(stoi(aux[i]) - 1);
                                    }
                                    break;
                                    // posicao 1 --> indice de mapamento de textura
                                case 1:
                                    if (aux[i].empty()) {
                                        if (vertices > 3)
                                            fAux->texts.push_back(-1);
                                        else
                                            f->texts.push_back(-1);
                                    } else {
                                        if (vertices > 3)
                                            fAux->texts.push_back(stoi(aux[i]) - 1);
                                        else
                                            f->texts.push_back(stoi(aux[i]) - 1);
                                    }
                                    break;
                                    // posicao 2 --> indice de normais
                                case 2:
                                    if (aux[i].empty()) {
                                        if (vertices > 3)
                                            fAux->norms.push_back(-1);
                                        else
                                            f->norms.push_back(-1);
                                    } else {
                                        if (vertices > 3)
                                            fAux->norms.push_back(stoi(aux[i]) - 1);
                                        else
                                            f->norms.push_back(stoi(aux[i]) - 1);
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                    }

                    // Adiciona face no grupo
                    g->faces.push_back(f);
                    if (vertices > 3) {
                        for (int i = 0; i < 3; i++) {
                            if (i == discartPointId) continue;
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
            mesh->calculeDistanceScale(maxBoundBox, minBoundBox);

            for (int i = 0; i < mesh->groups.size(); i++) {
                for (int j = 0; j < materials.size(); j++) {

                    if (mesh->groups[i]->materialName == materials[j]->materialName) {
                        mesh->groups[i]->material = materials[j];
                    }
                }
            }


            return mesh;
        }
        return nullptr;
    }

};


