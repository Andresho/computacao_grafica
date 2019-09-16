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
};