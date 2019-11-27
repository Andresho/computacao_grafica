#pragma once


class Material {
public:
    vec3 * Kd;
    vec3 * Ka;
    vec3 * Ks;

    float Ns;//shines

    string materialName;

    GLuint textureId;

    void bind(GLuint shaderProgram) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i((glGetUniformLocation(shaderProgram, "tex")), 0);
    }

    void createTexture(const char *filename, bool useAlpha) {

        glGenTextures(1, &textureId);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//ou GL_REPEAT GL_CLAMP_TO_BORDER
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//ou GL_LINEAR GL_NEAREST
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load and generate the texture
        int width, height, nrChannels;
        nrChannels = 0;

        unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
//		unsigned char *data = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            string message = "Falha ao carregar images. ";
            printf("%s", message.c_str());
            std::cout << filename << std::endl;
        }

        stbi_image_free(data);
    }

    ~Material() {
    };
};