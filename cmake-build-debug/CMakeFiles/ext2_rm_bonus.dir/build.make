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
include CMakeFiles/ext2_rm_bonus.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ext2_rm_bonus.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ext2_rm_bonus.dir/flags.make

CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o: CMakeFiles/ext2_rm_bonus.dir/flags.make
CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o: ../ext2_rm_bonus.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o   -c /Users/liallen/CLionProjects/a4/ext2_rm_bonus.c

CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/liallen/CLionProjects/a4/ext2_rm_bonus.c > CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.i

CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/liallen/CLionProjects/a4/ext2_rm_bonus.c -o CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.s

CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o.requires:

.PHONY : CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o.requires

CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o.provides: CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o.requires
	$(MAKE) -f CMakeFiles/ext2_rm_bonus.dir/build.make CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o.provides.build
.PHONY : CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o.provides

CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o.provides.build: CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o


CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o: CMakeFiles/ext2_rm_bonus.dir/flags.make
CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o: ../ext2_utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o   -c /Users/liallen/CLionProjects/a4/ext2_utils.c

CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/liallen/CLionProjects/a4/ext2_utils.c > CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.i

CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/liallen/CLionProjects/a4/ext2_utils.c -o CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.s

CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o.requires:

.PHONY : CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o.requires

CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o.provides: CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o.requires
	$(MAKE) -f CMakeFiles/ext2_rm_bonus.dir/build.make CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o.provides.build
.PHONY : CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o.provides

CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o.provides.build: CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o


# Object files for target ext2_rm_bonus
ext2_rm_bonus_OBJECTS = \
"CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o" \
"CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o"

# External object files for target ext2_rm_bonus
ext2_rm_bonus_EXTERNAL_OBJECTS =

ext2_rm_bonus: CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o
ext2_rm_bonus: CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o
ext2_rm_bonus: CMakeFiles/ext2_rm_bonus.dir/build.make
ext2_rm_bonus: CMakeFiles/ext2_rm_bonus.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ext2_rm_bonus"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ext2_rm_bonus.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ext2_rm_bonus.dir/build: ext2_rm_bonus

.PHONY : CMakeFiles/ext2_rm_bonus.dir/build

CMakeFiles/ext2_rm_bonus.dir/requires: CMakeFiles/ext2_rm_bonus.dir/ext2_rm_bonus.c.o.requires
CMakeFiles/ext2_rm_bonus.dir/requires: CMakeFiles/ext2_rm_bonus.dir/ext2_utils.c.o.requires

.PHONY : CMakeFiles/ext2_rm_bonus.dir/requires

CMakeFiles/ext2_rm_bonus.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ext2_rm_bonus.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ext2_rm_bonus.dir/clean

CMakeFiles/ext2_rm_bonus.dir/depend:
	cd /Users/liallen/CLionProjects/a4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/liallen/CLionProjects/a4 /Users/liallen/CLionProjects/a4 /Users/liallen/CLionProjects/a4/cmake-build-debug /Users/liallen/CLionProjects/a4/cmake-build-debug /Users/liallen/CLionProjects/a4/cmake-build-debug/CMakeFiles/ext2_rm_bonus.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ext2_rm_bonus.dir/depend
