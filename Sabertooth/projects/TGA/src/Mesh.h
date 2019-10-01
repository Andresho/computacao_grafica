#pragma once

class Mesh
{
public:
	vector<vec3*> vertex;
	vector<vec3*> normals;
	vector<vec2*> mappings;
	vector<Group*> groups;

    float distanceScale;

    void calculeDistanceScale( vec3 max, vec3 min){
        distanceScale = sqrt(pow(max.x-min.x,2)+pow(max.y-min.y,2)+pow(max.z-min.z,2));
    }

	~Mesh() {
	};

    void loadVertices() {
        Group *g;
        Face *f;
        for (int i = 0; i < this->groups.size(); i++) {
            g = this->groups[i];

            vector<float> vs;
            vector<float> vts;
            vector<float> vns;

            for (int j = 0; j < g->faces.size(); j++) {
                f = g->faces[j];

                // para vertices
                for(int k = 0; k< (f->verts.size()); k++){
                    int ind_v = f->verts[k];
                    if(ind_v>-1){
                        vec3* v = this->vertex[ind_v];
                        vs.push_back(v->x);
                        vs.push_back(v->y);
                        vs.push_back(v->z);
                    }
                }

                // para normais
                for(int n = 0; n<(f->norms.size()); n++){
                    int ind_n = f->norms[n];
                    if(ind_n>-1) {
                        vec3 *vn = this->normals[ind_n];
                        vns.push_back(vn->x);
                        vns.push_back(vn->y);
                        vns.push_back(vn->z);
                    }
                }

                // para texturas
                for(int t = 0; t< (f->texts.size()); t++){
                    int ind_t = f->texts[t];
                    if(ind_t>-1) {
                        vec2 *vt = this->mappings[ind_t];
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
};