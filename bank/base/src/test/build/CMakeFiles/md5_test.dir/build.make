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
include CMakeFiles/md5_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/md5_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/md5_test.dir/flags.make

CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o: CMakeFiles/md5_test.dir/flags.make
CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o: /home/zhihzen/base/src/test/md5_test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhihzen/base/src/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o"
	gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o   -c /home/zhihzen/base/src/test/md5_test.c

CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.i"
	gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zhihzen/base/src/test/md5_test.c > CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.i

CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.s"
	gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zhihzen/base/src/test/md5_test.c -o CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.s

CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o.requires:

.PHONY : CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o.requires

CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o.provides: CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o.requires
	$(MAKE) -f CMakeFiles/md5_test.dir/build.make CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o.provides.build
.PHONY : CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o.provides

CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o.provides.build: CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o


# Object files for target md5_test
md5_test_OBJECTS = \
"CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o"

# External object files for target md5_test
md5_test_EXTERNAL_OBJECTS =

md5_test: CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o
md5_test: CMakeFiles/md5_test.dir/build.make
md5_test: CMakeFiles/md5_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zhihzen/base/src/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable md5_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/md5_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/md5_test.dir/build: md5_test

.PHONY : CMakeFiles/md5_test.dir/build

CMakeFiles/md5_test.dir/requires: CMakeFiles/md5_test.dir/home/zhihzen/base/src/test/md5_test.c.o.requires

.PHONY : CMakeFiles/md5_test.dir/requires

CMakeFiles/md5_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/md5_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/md5_test.dir/clean

CMakeFiles/md5_test.dir/depend:
	cd /home/zhihzen/base/src/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhihzen/base/src/test/cmake /home/zhihzen/base/src/test/cmake /home/zhihzen/base/src/test/build /home/zhihzen/base/src/test/build /home/zhihzen/base/src/test/build/CMakeFiles/md5_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/md5_test.dir/depend

