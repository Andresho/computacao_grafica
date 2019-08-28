#pragma once

class Group
{
public:
	string name;
	string material;

	GLuint vbo;
	GLuint vao;

	int vertexSize = 0;

	vector<Face*> faces;
	Group(string name, string material, vector<Face*> faces) {
		this->name = name;
		this->material = material;
		this->faces = faces;
	};
	~Group() {};
};

