#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cmath>

//To GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#define STB_IMAGE_IMPLEMENTATION

#include "libs/stb_image.h"

using namespace std;

#include "./src/Material.h"
#include "./src/Face.h"
#include "./src/Group.h"
#include "./src/Mesh.h"
#include "./src/Obj3d.h"
#include "./src/FileReader.h"
#include "./src/FileWriter.h"
#include "./src/Shader.h"
