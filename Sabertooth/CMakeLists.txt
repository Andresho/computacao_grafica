cmake_minimum_required(VERSION 3.10)
project(TGA_CG)

SET(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(TGA_CG
        CMakeLists.txt
        ../External/SOIL2/include/SOIL.h
        projects/TGA/TGA.h
        projects/TGA/shaders/core.vert
        projects/TGA/shaders/core.frag
        projects/TGA/src/Face.h
        projects/TGA/src/Group.h
        projects/TGA/src/Mesh.h
        projects/TGA/src/Obj3d.h
        projects/TGA/src/Material.h
        main.cpp)

add_executable(SimpleTriangle
        CMakeLists.txt
        ../External/SOIL2/include/SOIL.h
        projects/TGA/TGA.h
        projects/TGA/shaders/core.vert
        projects/TGA/shaders/core.frag
        projects/TGA/src/Face.h
        projects/TGA/src/Group.h
        projects/TGA/src/Mesh.h
        projects/TGA/src/Obj3d.h
        projects/TGA/src/Material.h
        main.cpp)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_library(Cocoa REQUIRED)
find_library(CoreVideo REQUIRED)
find_library(IOKit REQUIRED)
find_package(GLFW3 REQUIRED)

target_link_libraries(TGA_CG /usr/local/lib/libglfw.3.3.dylib ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
target_include_directories(TGA_CG PRIVATE ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} /usr/local/include/glm)

target_link_libraries(SimpleTriangle /usr/local/lib/libglfw.3.3.dylib ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
target_include_directories(SimpleTriangle PRIVATE ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} /usr/local/include/glm)
