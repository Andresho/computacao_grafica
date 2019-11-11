#pragma once

class FileWriter {
public:
    FileWriter() {

    }
    void writeArchivesOBJandMTL(
            vector<float> &internalCurvePointsX,
            vector<float> &internalCurvePointsY,
            vector<float> &internalCurvePointsZ,
            vector<float> &externalCurvePointsX,
            vector<float> &externalCurvePointsY,
            vector<float> &externalCurvePointsZ,
            vector<glm::vec3*> &normals
    ) {
        ofstream objFileStream;
        objFileStream.open("projects/CurvesEditor/export/pista.obj");

        objFileStream << "mtllib pista.mtl" << endl;

        // escreve os vertices da curva interna
        for (int i = 0; i < internalCurvePointsX.size(); i += 1) {
            // o X e o Z ficam invertidos devido a geometria 3D
            objFileStream << "v " << internalCurvePointsX[i] << " " << internalCurvePointsZ[i] * 100 << " " << internalCurvePointsY[i] - 300 << endl;
        }

        // escreve os vertices da curva externa
        for (int i = 0; i < externalCurvePointsX.size(); i += 1) {
            // o X e o Z ficam invertidos devido a geometria 3D
            objFileStream << "v " << externalCurvePointsX[i] << " " << externalCurvePointsZ[i] * 100 << " " << externalCurvePointsY[i] - 300 << endl;
        }

        // define e escreve as coordenadas de textura
        objFileStream << "vt " << 0 << " " << 0 << endl;
        objFileStream << "vt " << 1 << " " << 0 << endl;
        objFileStream << "vt " << 1 << " " << 1 << endl;
        objFileStream << "vt " << 0 << " " << 1 << endl;

        // escreve as normais no arquivo
        for (int i = 0; i < normals.size(); i += 1) {
            float zNormal = ((1/normals[i]->z)-1) * (-1);
            objFileStream << "vn " << normals[i]->x << " " << normals[i]->y << " " << zNormal << endl;
        }

        // escreve o grupo
        objFileStream << "g Pista" << endl;
        objFileStream << "usemtl Pista" << endl;

        // escrever os indices das faces
        int qtdId = 0;
        int points = internalCurvePointsX.size();
        // comeca a escrever em 1 pois o padrao das faces é 1 e nao 0 nos arquivos OB
        for (int i = 1; i < points; i += 1) {
            qtdId += 1;
            objFileStream << "f " << i << "/3/" << qtdId << " " << i + points << "/1/" << qtdId << " " << i + points + 1 << "/2/" << qtdId << endl;
            objFileStream << "f " << i + points + 1 << "/2/" << qtdId << " " << i + 1 << "/4/" << qtdId << " " << i << "/3/" << qtdId << endl;
        }

        objFileStream.close();

        // escrever o arquivo de material
        ofstream mtlFileStream;
        mtlFileStream.open("projects/CurvesEditor/export/pista.mtl");

        mtlFileStream << "newmtl Pista\n" << endl;

        mtlFileStream << "map_Kd terra.jpg" << endl;

        mtlFileStream << "Kd 0.50 0.50 0.50" << endl;

        mtlFileStream << "Ka 0.15 0.15 0.15" << endl;

        mtlFileStream << "Ks 1.00 1.00 1.00" << endl;

        mtlFileStream << "Ns 0.06" << endl;

        mtlFileStream.close();
    }

};


