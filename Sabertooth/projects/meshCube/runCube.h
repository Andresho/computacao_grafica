#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "SOIL.h"

//To GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load image Libraries
//#define STB_IMAGE_IMPLEMENTATION
#ifdef __APPLE__
	#define STB_IMAGE_IMPLEMENTATION
	#include "./src/stb_image.h"
#elif _WIN64
	#include "./src/stb_image.h"
#endif

using namespace glm;
using namespace std;

#include "./src/Material.h"
#include "./src/Face.h"
#include "./src/Group.h"
#include "./src/Mesh.h"
#include "./src/Obj3d.h"

int runCube();

