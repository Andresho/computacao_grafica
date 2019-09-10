cmake_minimum_required(VERSION 3.10)
project(MeshCube)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)



add_executable(MeshCube
        CMakeLists.txt
        projects/meshCube/shaders/core.vert
        projects/meshCube/shaders/core.frag
        projects/meshCube/objs/cube.obj
        projects/meshCube/run.h
        projects/meshCube/run.cpp
        projects/meshCube/src/Face.h
        projects/meshCube/src/Group.h
        projects/meshCube/src/Mesh.h
        main.cpp
        )

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_library(Cocoa REQUIRED)
find_library(CoreVideo REQUIRED)
find_library(IOKit REQUIRED)
find_package(GLFW3 REQUIRED)

##/usr/local/lib/libglfw.3.2.dylib

target_link_libraries(MeshCube /usr/local/lib/libglfw.3.3.dylib ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
target_include_directories(MeshCube PRIVATE ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} /usr/local/include/glm)
