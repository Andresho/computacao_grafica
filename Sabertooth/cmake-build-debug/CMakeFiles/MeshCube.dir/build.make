# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/gian/GitHub/computacao_grafica/Sabertooth

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/gian/GitHub/computacao_grafica/Sabertooth/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MeshCube.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MeshCube.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MeshCube.dir/flags.make

CMakeFiles/MeshCube.dir/main.cpp.o: CMakeFiles/MeshCube.dir/flags.make
CMakeFiles/MeshCube.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gian/GitHub/computacao_grafica/Sabertooth/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MeshCube.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MeshCube.dir/main.cpp.o -c /Users/gian/GitHub/computacao_grafica/Sabertooth/main.cpp

CMakeFiles/MeshCube.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MeshCube.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gian/GitHub/computacao_grafica/Sabertooth/main.cpp > CMakeFiles/MeshCube.dir/main.cpp.i

CMakeFiles/MeshCube.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MeshCube.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gian/GitHub/computacao_grafica/Sabertooth/main.cpp -o CMakeFiles/MeshCube.dir/main.cpp.s

# Object files for target MeshCube
MeshCube_OBJECTS = \
"CMakeFiles/MeshCube.dir/main.cpp.o"

# External object files for target MeshCube
MeshCube_EXTERNAL_OBJECTS =

MeshCube: CMakeFiles/MeshCube.dir/main.cpp.o
MeshCube: CMakeFiles/MeshCube.dir/build.make
MeshCube: /usr/local/lib/libglfw.3.3.dylib
MeshCube: /usr/local/lib/libGLEW.dylib
MeshCube: CMakeFiles/MeshCube.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/gian/GitHub/computacao_grafica/Sabertooth/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MeshCube"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MeshCube.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MeshCube.dir/build: MeshCube

.PHONY : CMakeFiles/MeshCube.dir/build

CMakeFiles/MeshCube.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MeshCube.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MeshCube.dir/clean

CMakeFiles/MeshCube.dir/depend:
	cd /Users/gian/GitHub/computacao_grafica/Sabertooth/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/gian/GitHub/computacao_grafica/Sabertooth /Users/gian/GitHub/computacao_grafica/Sabertooth /Users/gian/GitHub/computacao_grafica/Sabertooth/cmake-build-debug /Users/gian/GitHub/computacao_grafica/Sabertooth/cmake-build-debug /Users/gian/GitHub/computacao_grafica/Sabertooth/cmake-build-debug/CMakeFiles/MeshCube.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MeshCube.dir/depend

