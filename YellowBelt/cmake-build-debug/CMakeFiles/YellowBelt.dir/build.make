# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /home/ilya/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/201.7223.86/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/ilya/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/201.7223.86/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ilya/CLionProjects/Coursera/YellowBelt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ilya/CLionProjects/Coursera/YellowBelt/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/YellowBelt.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/YellowBelt.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/YellowBelt.dir/flags.make

CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.o: CMakeFiles/YellowBelt.dir/flags.make
CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.o: ../MassOfBlocks.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ilya/CLionProjects/Coursera/YellowBelt/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.o -c /home/ilya/CLionProjects/Coursera/YellowBelt/MassOfBlocks.cpp

CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ilya/CLionProjects/Coursera/YellowBelt/MassOfBlocks.cpp > CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.i

CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ilya/CLionProjects/Coursera/YellowBelt/MassOfBlocks.cpp -o CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.s

# Object files for target YellowBelt
YellowBelt_OBJECTS = \
"CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.o"

# External object files for target YellowBelt
YellowBelt_EXTERNAL_OBJECTS =

YellowBelt: CMakeFiles/YellowBelt.dir/MassOfBlocks.cpp.o
YellowBelt: CMakeFiles/YellowBelt.dir/build.make
YellowBelt: CMakeFiles/YellowBelt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ilya/CLionProjects/Coursera/YellowBelt/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable YellowBelt"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/YellowBelt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/YellowBelt.dir/build: YellowBelt

.PHONY : CMakeFiles/YellowBelt.dir/build

CMakeFiles/YellowBelt.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/YellowBelt.dir/cmake_clean.cmake
.PHONY : CMakeFiles/YellowBelt.dir/clean

CMakeFiles/YellowBelt.dir/depend:
	cd /home/ilya/CLionProjects/Coursera/YellowBelt/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ilya/CLionProjects/Coursera/YellowBelt /home/ilya/CLionProjects/Coursera/YellowBelt /home/ilya/CLionProjects/Coursera/YellowBelt/cmake-build-debug /home/ilya/CLionProjects/Coursera/YellowBelt/cmake-build-debug /home/ilya/CLionProjects/Coursera/YellowBelt/cmake-build-debug/CMakeFiles/YellowBelt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/YellowBelt.dir/depend

