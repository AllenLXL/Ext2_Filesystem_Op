# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
include CMakeFiles/a4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/a4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/a4.dir/flags.make

CMakeFiles/a4.dir/ext2_cp.c.o: CMakeFiles/a4.dir/flags.make
CMakeFiles/a4.dir/ext2_cp.c.o: ../ext2_cp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/a4.dir/ext2_cp.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/a4.dir/ext2_cp.c.o   -c /Users/liallen/CLionProjects/a4/ext2_cp.c

CMakeFiles/a4.dir/ext2_cp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/a4.dir/ext2_cp.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/liallen/CLionProjects/a4/ext2_cp.c > CMakeFiles/a4.dir/ext2_cp.c.i

CMakeFiles/a4.dir/ext2_cp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/a4.dir/ext2_cp.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/liallen/CLionProjects/a4/ext2_cp.c -o CMakeFiles/a4.dir/ext2_cp.c.s

CMakeFiles/a4.dir/ext2_cp.c.o.requires:

.PHONY : CMakeFiles/a4.dir/ext2_cp.c.o.requires

CMakeFiles/a4.dir/ext2_cp.c.o.provides: CMakeFiles/a4.dir/ext2_cp.c.o.requires
	$(MAKE) -f CMakeFiles/a4.dir/build.make CMakeFiles/a4.dir/ext2_cp.c.o.provides.build
.PHONY : CMakeFiles/a4.dir/ext2_cp.c.o.provides

CMakeFiles/a4.dir/ext2_cp.c.o.provides.build: CMakeFiles/a4.dir/ext2_cp.c.o


CMakeFiles/a4.dir/ext2_mkdir.c.o: CMakeFiles/a4.dir/flags.make
CMakeFiles/a4.dir/ext2_mkdir.c.o: ../ext2_mkdir.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/a4.dir/ext2_mkdir.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/a4.dir/ext2_mkdir.c.o   -c /Users/liallen/CLionProjects/a4/ext2_mkdir.c

CMakeFiles/a4.dir/ext2_mkdir.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/a4.dir/ext2_mkdir.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/liallen/CLionProjects/a4/ext2_mkdir.c > CMakeFiles/a4.dir/ext2_mkdir.c.i

CMakeFiles/a4.dir/ext2_mkdir.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/a4.dir/ext2_mkdir.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/liallen/CLionProjects/a4/ext2_mkdir.c -o CMakeFiles/a4.dir/ext2_mkdir.c.s

CMakeFiles/a4.dir/ext2_mkdir.c.o.requires:

.PHONY : CMakeFiles/a4.dir/ext2_mkdir.c.o.requires

CMakeFiles/a4.dir/ext2_mkdir.c.o.provides: CMakeFiles/a4.dir/ext2_mkdir.c.o.requires
	$(MAKE) -f CMakeFiles/a4.dir/build.make CMakeFiles/a4.dir/ext2_mkdir.c.o.provides.build
.PHONY : CMakeFiles/a4.dir/ext2_mkdir.c.o.provides

CMakeFiles/a4.dir/ext2_mkdir.c.o.provides.build: CMakeFiles/a4.dir/ext2_mkdir.c.o


CMakeFiles/a4.dir/ext2_utils.c.o: CMakeFiles/a4.dir/flags.make
CMakeFiles/a4.dir/ext2_utils.c.o: ../ext2_utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/a4.dir/ext2_utils.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/a4.dir/ext2_utils.c.o   -c /Users/liallen/CLionProjects/a4/ext2_utils.c

CMakeFiles/a4.dir/ext2_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/a4.dir/ext2_utils.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/liallen/CLionProjects/a4/ext2_utils.c > CMakeFiles/a4.dir/ext2_utils.c.i

CMakeFiles/a4.dir/ext2_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/a4.dir/ext2_utils.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/liallen/CLionProjects/a4/ext2_utils.c -o CMakeFiles/a4.dir/ext2_utils.c.s

CMakeFiles/a4.dir/ext2_utils.c.o.requires:

.PHONY : CMakeFiles/a4.dir/ext2_utils.c.o.requires

CMakeFiles/a4.dir/ext2_utils.c.o.provides: CMakeFiles/a4.dir/ext2_utils.c.o.requires
	$(MAKE) -f CMakeFiles/a4.dir/build.make CMakeFiles/a4.dir/ext2_utils.c.o.provides.build
.PHONY : CMakeFiles/a4.dir/ext2_utils.c.o.provides

CMakeFiles/a4.dir/ext2_utils.c.o.provides.build: CMakeFiles/a4.dir/ext2_utils.c.o


# Object files for target a4
a4_OBJECTS = \
"CMakeFiles/a4.dir/ext2_cp.c.o" \
"CMakeFiles/a4.dir/ext2_mkdir.c.o" \
"CMakeFiles/a4.dir/ext2_utils.c.o"

# External object files for target a4
a4_EXTERNAL_OBJECTS =

a4: CMakeFiles/a4.dir/ext2_cp.c.o
a4: CMakeFiles/a4.dir/ext2_mkdir.c.o
a4: CMakeFiles/a4.dir/ext2_utils.c.o
a4: CMakeFiles/a4.dir/build.make
a4: CMakeFiles/a4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable a4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/a4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/a4.dir/build: a4

.PHONY : CMakeFiles/a4.dir/build

CMakeFiles/a4.dir/requires: CMakeFiles/a4.dir/ext2_cp.c.o.requires
CMakeFiles/a4.dir/requires: CMakeFiles/a4.dir/ext2_mkdir.c.o.requires
CMakeFiles/a4.dir/requires: CMakeFiles/a4.dir/ext2_utils.c.o.requires

.PHONY : CMakeFiles/a4.dir/requires

CMakeFiles/a4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/a4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/a4.dir/clean

CMakeFiles/a4.dir/depend:
	cd /Users/liallen/CLionProjects/a4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/liallen/CLionProjects/a4 /Users/liallen/CLionProjects/a4 /Users/liallen/CLionProjects/a4/cmake-build-debug /Users/liallen/CLionProjects/a4/cmake-build-debug /Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles/a4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/a4.dir/depend

