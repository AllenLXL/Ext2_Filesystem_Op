# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/liallen/CLionProjects/a4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/liallen/CLionProjects/a4/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/readimage.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/readimage.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/readimage.dir/flags.make

CMakeFiles/readimage.dir/readimage.c.o: CMakeFiles/readimage.dir/flags.make
CMakeFiles/readimage.dir/readimage.c.o: ../readimage.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/readimage.dir/readimage.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/readimage.dir/readimage.c.o   -c /Users/liallen/CLionProjects/a4/readimage.c

CMakeFiles/readimage.dir/readimage.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/readimage.dir/readimage.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/liallen/CLionProjects/a4/readimage.c > CMakeFiles/readimage.dir/readimage.c.i

CMakeFiles/readimage.dir/readimage.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/readimage.dir/readimage.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/liallen/CLionProjects/a4/readimage.c -o CMakeFiles/readimage.dir/readimage.c.s

CMakeFiles/readimage.dir/readimage.c.o.requires:

.PHONY : CMakeFiles/readimage.dir/readimage.c.o.requires

CMakeFiles/readimage.dir/readimage.c.o.provides: CMakeFiles/readimage.dir/readimage.c.o.requires
	$(MAKE) -f CMakeFiles/readimage.dir/build.make CMakeFiles/readimage.dir/readimage.c.o.provides.build
.PHONY : CMakeFiles/readimage.dir/readimage.c.o.provides

CMakeFiles/readimage.dir/readimage.c.o.provides.build: CMakeFiles/readimage.dir/readimage.c.o


# Object files for target readimage
readimage_OBJECTS = \
"CMakeFiles/readimage.dir/readimage.c.o"

# External object files for target readimage
readimage_EXTERNAL_OBJECTS =

readimage: CMakeFiles/readimage.dir/readimage.c.o
readimage: CMakeFiles/readimage.dir/build.make
readimage: CMakeFiles/readimage.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable readimage"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/readimage.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/readimage.dir/build: readimage

.PHONY : CMakeFiles/readimage.dir/build

CMakeFiles/readimage.dir/requires: CMakeFiles/readimage.dir/readimage.c.o.requires

.PHONY : CMakeFiles/readimage.dir/requires

CMakeFiles/readimage.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/readimage.dir/cmake_clean.cmake
.PHONY : CMakeFiles/readimage.dir/clean

CMakeFiles/readimage.dir/depend:
	cd /Users/liallen/CLionProjects/a4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/liallen/CLionProjects/a4 /Users/liallen/CLionProjects/a4 /Users/liallen/CLionProjects/a4/cmake-build-debug /Users/liallen/CLionProjects/a4/cmake-build-debug /Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles/readimage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/readimage.dir/depend
