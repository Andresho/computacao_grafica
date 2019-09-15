class Material
{
public:
    vector<vec3*> Kd;
    vector<vec3*> Ka;
    vector<vec3*> Ks;

    float Ns;

    string materialName;

    GLuint textureId;

    ~Material() {
    };
};