#pragma once

class Mesh
{
public:
	vector<vec3*> vertex;
	vector<vec3*> normals;
	vector<vec2*> mappings;

	vector<Group*> groups;


	~Mesh() {
	};
};