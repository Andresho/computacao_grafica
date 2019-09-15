#pragma once
#ifndef RUNCUBE_H  
#define RUNCUBE_H

#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
//To GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

#include "./src/Face.h"
#include "./src/Group.h"
#include "./src/Mesh.h"
#include "./src/Material.h"
#include "./src/Obj3d.h"

int runCube();

#endif