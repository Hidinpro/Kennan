# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/zhihzen/base/src/test/cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zhihzen/base/src/test/build

# Include any dependencies generated for this target.
include CMakeFiles/json_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/json_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/json_test.dir/flags.make

CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o: CMakeFiles/json_test.dir/flags.make
CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o: /home/zhihzen/base/src/test/cJSON_test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhihzen/base/src/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o"
	gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o   -c /home/zhihzen/base/src/test/cJSON_test.c

CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.i"
	gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zhihzen/base/src/test/cJSON_test.c > CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.i

CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.s"
	gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zhihzen/base/src/test/cJSON_test.c -o CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.s

CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o.requires:

.PHONY : CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o.requires

CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o.provides: CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o.requires
	$(MAKE) -f CMakeFiles/json_test.dir/build.make CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o.provides.build
.PHONY : CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o.provides

CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o.provides.build: CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o


# Object files for target json_test
json_test_OBJECTS = \
"CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o"

# External object files for target json_test
json_test_EXTERNAL_OBJECTS =

json_test: CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o
json_test: CMakeFiles/json_test.dir/build.make
json_test: CMakeFiles/json_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zhihzen/base/src/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable json_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/json_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/json_test.dir/build: json_test

.PHONY : CMakeFiles/json_test.dir/build

CMakeFiles/json_test.dir/requires: CMakeFiles/json_test.dir/home/zhihzen/base/src/test/cJSON_test.c.o.requires

.PHONY : CMakeFiles/json_test.dir/requires

CMakeFiles/json_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/json_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/json_test.dir/clean

CMakeFiles/json_test.dir/depend:
	cd /home/zhihzen/base/src/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhihzen/base/src/test/cmake /home/zhihzen/base/src/test/cmake /home/zhihzen/base/src/test/build /home/zhihzen/base/src/test/build /home/zhihzen/base/src/test/build/CMakeFiles/json_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/json_test.dir/depend

