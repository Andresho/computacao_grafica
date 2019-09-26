#pragma once
class Obj3d
{
public:
	Mesh *mesh;

	float xCenter;
	float yCenter;
	float zCenter;

	glm::mat4 matrix_translaction;
	glm::mat4 matrix_rotation;
	glm::mat4 matrix_scala;
	glm::mat4 transformations;

	Obj3d(Mesh *mesh, float initialPosX, float initialPosY, float initialPosZ) {
		this->mesh = mesh;
		
		this->xCenter = 0.0f;
		this->yCenter = 0.0f;
		this->zCenter = 0.0f;

		this->matrix_translaction = glm::mat4(1);
		this->matrix_rotation = glm::mat4(1);
		this->matrix_scala = glm::mat4(1);

		this->move(initialPosX, initialPosY, initialPosZ);
		this->transform();
	};

	void move(float x, float y, float z) {
		matrix_translaction = glm::translate(matrix_translaction, glm::vec3(x, y, z));
		xCenter += x;
		yCenter += y;
		zCenter += z;
		transform();
	}

	void rotate(float rad) {
		matrix_rotation = glm::rotate(matrix_rotation, glm::radians(rad), glm::vec3(0, 1, 0));
		transform();
	}

    void scale(float x,float y,float z) {
        matrix_scala = glm::scale(matrix_scala, glm::vec3(x, y, z));
        transform();
    }

    void scale(float s) {
        matrix_scala = glm::scale(matrix_scala, glm::vec3(s, s, s));
        transform();
    }

    void transform() {
		transformations = matrix_translaction * matrix_rotation * matrix_scala;
	}

	~Obj3d() {};
};
