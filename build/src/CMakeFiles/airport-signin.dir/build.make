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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /opt/airport-signin

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /opt/airport-signin/build

# Include any dependencies generated for this target.
include src/CMakeFiles/airport-signin.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/airport-signin.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/airport-signin.dir/flags.make

src/CMakeFiles/airport-signin.dir/main.cpp.o: src/CMakeFiles/airport-signin.dir/flags.make
src/CMakeFiles/airport-signin.dir/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/opt/airport-signin/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/airport-signin.dir/main.cpp.o"
	cd /opt/airport-signin/build/src && /bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/airport-signin.dir/main.cpp.o -c /opt/airport-signin/src/main.cpp

src/CMakeFiles/airport-signin.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/airport-signin.dir/main.cpp.i"
	cd /opt/airport-signin/build/src && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /opt/airport-signin/src/main.cpp > CMakeFiles/airport-signin.dir/main.cpp.i

src/CMakeFiles/airport-signin.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/airport-signin.dir/main.cpp.s"
	cd /opt/airport-signin/build/src && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /opt/airport-signin/src/main.cpp -o CMakeFiles/airport-signin.dir/main.cpp.s

# Object files for target airport-signin
airport__signin_OBJECTS = \
"CMakeFiles/airport-signin.dir/main.cpp.o"

# External object files for target airport-signin
airport__signin_EXTERNAL_OBJECTS =

src/airport-signin: src/CMakeFiles/airport-signin.dir/main.cpp.o
src/airport-signin: src/CMakeFiles/airport-signin.dir/build.make
src/airport-signin: src/CMakeFiles/airport-signin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/opt/airport-signin/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable airport-signin"
	cd /opt/airport-signin/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/airport-signin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/airport-signin.dir/build: src/airport-signin

.PHONY : src/CMakeFiles/airport-signin.dir/build

src/CMakeFiles/airport-signin.dir/clean:
	cd /opt/airport-signin/build/src && $(CMAKE_COMMAND) -P CMakeFiles/airport-signin.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/airport-signin.dir/clean

src/CMakeFiles/airport-signin.dir/depend:
	cd /opt/airport-signin/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /opt/airport-signin /opt/airport-signin/src /opt/airport-signin/build /opt/airport-signin/build/src /opt/airport-signin/build/src/CMakeFiles/airport-signin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/airport-signin.dir/depend
