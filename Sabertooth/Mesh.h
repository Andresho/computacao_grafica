#pragma once

class Mesh
{
public:
	vector<vec3> vertex;
	vector<vec3> normals;
	vector<vec2> mappings;

	vector<Group*> groups;

	Mesh(vector<vec3> vertex, vector<vec3> normals, vector<vec2> mappings, vector<Group*> groups) {
		this->vertex = vertex;
		this->normals = normals;
		this->mappings = mappings;
		this->groups = groups;
	};
	~Mesh() {
	};
};