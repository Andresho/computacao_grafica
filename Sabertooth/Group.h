#pragma once

class Group
{
public:
	string name;
	string material;

	GLuint vbo;
	GLuint vao;

	int numVertices = 0;

	vector<Face*> faces;

	Group(string name, string material) {
		this->name = name;
		this->material = material;
	};
	~Group() {};
};

