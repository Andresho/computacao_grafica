cmake_minimum_required(VERSION 3.10)
project(TGA_CG)
project(TrackReader)
project(CurvesEditor)

SET(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(TGA_CG
        CMakeLists.txt
        projects/TGA/TGA.h
        projects/TGA/shaders/core.vert
        projects/TGA/shaders/core.frag
        projects/TGA/src/Face.h
        projects/TGA/src/Group.h
        projects/TGA/src/Mesh.h
        projects/TGA/src/Obj3d.h
        projects/TGA/src/Material.h
        main.cpp)

add_executable(TrackReader
        CMakeLists.txt
        projects/TrackReader/TrackReader.h
        projects/TrackReader/shaders/core.vert
        projects/TrackReader/shaders/core.frag
        projects/TrackReader/src/Face.h
        projects/TrackReader/src/Group.h
        projects/TrackReader/src/Mesh.h
        projects/TrackReader/src/Obj3d.h
        projects/TrackReader/src/Material.h
        main.cpp)

add_executable(CurvesEditor
        CMakeLists.txt
        projects/TrackReader/TrackReader.h
        projects/TrackReader/shaders/core.vert
        projects/TrackReader/shaders/core.frag
        projects/TrackReader/src/Face.h
        projects/TrackReader/src/Group.h
        projects/TrackReader/src/Mesh.h
        projects/TrackReader/src/Obj3d.h
        projects/TrackReader/src/Material.h
        main.cpp)


find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_library(Cocoa REQUIRED)
find_library(CoreVideo REQUIRED)
find_library(IOKit REQUIRED)
find_package(GLFW3 REQUIRED)

target_link_libraries(TGA_CG /usr/local/lib/libglfw.3.3.dylib /usr/local/lib/libGLEW.2.1.dylib ${OPENGL_LIBRARIES})
target_include_directories(TGA_CG PRIVATE /usr/local/include)

target_link_libraries(TrackReader /usr/local/lib/libglfw.3.3.dylib /usr/local/lib/libGLEW.2.1.dylib ${OPENGL_LIBRARIES})
target_include_directories(TrackReader PRIVATE /usr/local/include)

target_link_libraries(CurvesEditor /usr/local/lib/libglfw.3.3.dylib /usr/local/lib/libGLEW.2.1.dylib ${OPENGL_LIBRARIES})
target_include_directories(CurvesEditor PRIVATE /usr/local/include)
