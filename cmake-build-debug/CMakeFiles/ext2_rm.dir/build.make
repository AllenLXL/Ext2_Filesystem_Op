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
include CMakeFiles/ext2_rm.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ext2_rm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ext2_rm.dir/flags.make

CMakeFiles/ext2_rm.dir/ext2_rm.c.o: CMakeFiles/ext2_rm.dir/flags.make
CMakeFiles/ext2_rm.dir/ext2_rm.c.o: ../ext2_rm.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ext2_rm.dir/ext2_rm.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ext2_rm.dir/ext2_rm.c.o   -c /Users/liallen/CLionProjects/a4/ext2_rm.c

CMakeFiles/ext2_rm.dir/ext2_rm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ext2_rm.dir/ext2_rm.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/liallen/CLionProjects/a4/ext2_rm.c > CMakeFiles/ext2_rm.dir/ext2_rm.c.i

CMakeFiles/ext2_rm.dir/ext2_rm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ext2_rm.dir/ext2_rm.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/liallen/CLionProjects/a4/ext2_rm.c -o CMakeFiles/ext2_rm.dir/ext2_rm.c.s

CMakeFiles/ext2_rm.dir/ext2_rm.c.o.requires:

.PHONY : CMakeFiles/ext2_rm.dir/ext2_rm.c.o.requires

CMakeFiles/ext2_rm.dir/ext2_rm.c.o.provides: CMakeFiles/ext2_rm.dir/ext2_rm.c.o.requires
	$(MAKE) -f CMakeFiles/ext2_rm.dir/build.make CMakeFiles/ext2_rm.dir/ext2_rm.c.o.provides.build
.PHONY : CMakeFiles/ext2_rm.dir/ext2_rm.c.o.provides

CMakeFiles/ext2_rm.dir/ext2_rm.c.o.provides.build: CMakeFiles/ext2_rm.dir/ext2_rm.c.o


CMakeFiles/ext2_rm.dir/ext2_utils.c.o: CMakeFiles/ext2_rm.dir/flags.make
CMakeFiles/ext2_rm.dir/ext2_utils.c.o: ../ext2_utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/ext2_rm.dir/ext2_utils.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ext2_rm.dir/ext2_utils.c.o   -c /Users/liallen/CLionProjects/a4/ext2_utils.c

CMakeFiles/ext2_rm.dir/ext2_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ext2_rm.dir/ext2_utils.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/liallen/CLionProjects/a4/ext2_utils.c > CMakeFiles/ext2_rm.dir/ext2_utils.c.i

CMakeFiles/ext2_rm.dir/ext2_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ext2_rm.dir/ext2_utils.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/liallen/CLionProjects/a4/ext2_utils.c -o CMakeFiles/ext2_rm.dir/ext2_utils.c.s

CMakeFiles/ext2_rm.dir/ext2_utils.c.o.requires:

.PHONY : CMakeFiles/ext2_rm.dir/ext2_utils.c.o.requires

CMakeFiles/ext2_rm.dir/ext2_utils.c.o.provides: CMakeFiles/ext2_rm.dir/ext2_utils.c.o.requires
	$(MAKE) -f CMakeFiles/ext2_rm.dir/build.make CMakeFiles/ext2_rm.dir/ext2_utils.c.o.provides.build
.PHONY : CMakeFiles/ext2_rm.dir/ext2_utils.c.o.provides

CMakeFiles/ext2_rm.dir/ext2_utils.c.o.provides.build: CMakeFiles/ext2_rm.dir/ext2_utils.c.o


# Object files for target ext2_rm
ext2_rm_OBJECTS = \
"CMakeFiles/ext2_rm.dir/ext2_rm.c.o" \
"CMakeFiles/ext2_rm.dir/ext2_utils.c.o"

# External object files for target ext2_rm
ext2_rm_EXTERNAL_OBJECTS =

ext2_rm: CMakeFiles/ext2_rm.dir/ext2_rm.c.o
ext2_rm: CMakeFiles/ext2_rm.dir/ext2_utils.c.o
ext2_rm: CMakeFiles/ext2_rm.dir/build.make
ext2_rm: CMakeFiles/ext2_rm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ext2_rm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ext2_rm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ext2_rm.dir/build: ext2_rm

.PHONY : CMakeFiles/ext2_rm.dir/build

CMakeFiles/ext2_rm.dir/requires: CMakeFiles/ext2_rm.dir/ext2_rm.c.o.requires
CMakeFiles/ext2_rm.dir/requires: CMakeFiles/ext2_rm.dir/ext2_utils.c.o.requires

.PHONY : CMakeFiles/ext2_rm.dir/requires

CMakeFiles/ext2_rm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ext2_rm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ext2_rm.dir/clean

CMakeFiles/ext2_rm.dir/depend:
	cd /Users/liallen/CLionProjects/a4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/liallen/CLionProjects/a4 /Users/liallen/CLionProjects/a4 /Users/liallen/CLionProjects/a4/cmake-build-debug /Users/liallen/CLionProjects/a4/cmake-build-debug /Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles/ext2_rm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ext2_rm.dir/depend

