# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build

# Include any dependencies generated for this target.
include test/kumipuyo/CMakeFiles/TestKumipuyo.dir/depend.make

# Include the progress variables for this target.
include test/kumipuyo/CMakeFiles/TestKumipuyo.dir/progress.make

# Include the compile flags for this target's objects.
include test/kumipuyo/CMakeFiles/TestKumipuyo.dir/flags.make

test/kumipuyo/CMakeFiles/TestKumipuyo.dir/test.cpp.o: test/kumipuyo/CMakeFiles/TestKumipuyo.dir/flags.make
test/kumipuyo/CMakeFiles/TestKumipuyo.dir/test.cpp.o: ../test/kumipuyo/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/kumipuyo/CMakeFiles/TestKumipuyo.dir/test.cpp.o"
	cd /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build/test/kumipuyo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TestKumipuyo.dir/test.cpp.o -c /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/test/kumipuyo/test.cpp

test/kumipuyo/CMakeFiles/TestKumipuyo.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TestKumipuyo.dir/test.cpp.i"
	cd /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build/test/kumipuyo && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/test/kumipuyo/test.cpp > CMakeFiles/TestKumipuyo.dir/test.cpp.i

test/kumipuyo/CMakeFiles/TestKumipuyo.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TestKumipuyo.dir/test.cpp.s"
	cd /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build/test/kumipuyo && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/test/kumipuyo/test.cpp -o CMakeFiles/TestKumipuyo.dir/test.cpp.s

# Object files for target TestKumipuyo
TestKumipuyo_OBJECTS = \
"CMakeFiles/TestKumipuyo.dir/test.cpp.o"

# External object files for target TestKumipuyo
TestKumipuyo_EXTERNAL_OBJECTS =

test/kumipuyo/TestKumipuyo: test/kumipuyo/CMakeFiles/TestKumipuyo.dir/test.cpp.o
test/kumipuyo/TestKumipuyo: test/kumipuyo/CMakeFiles/TestKumipuyo.dir/build.make
test/kumipuyo/TestKumipuyo: src/libBpuyoAI.a
test/kumipuyo/TestKumipuyo: /usr/local/lib/libgtest.a
test/kumipuyo/TestKumipuyo: /usr/local/lib/libgtest_main.a
test/kumipuyo/TestKumipuyo: /usr/local/lib/libgtest.a
test/kumipuyo/TestKumipuyo: test/kumipuyo/CMakeFiles/TestKumipuyo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable TestKumipuyo"
	cd /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build/test/kumipuyo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TestKumipuyo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/kumipuyo/CMakeFiles/TestKumipuyo.dir/build: test/kumipuyo/TestKumipuyo

.PHONY : test/kumipuyo/CMakeFiles/TestKumipuyo.dir/build

test/kumipuyo/CMakeFiles/TestKumipuyo.dir/clean:
	cd /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build/test/kumipuyo && $(CMAKE_COMMAND) -P CMakeFiles/TestKumipuyo.dir/cmake_clean.cmake
.PHONY : test/kumipuyo/CMakeFiles/TestKumipuyo.dir/clean

test/kumipuyo/CMakeFiles/TestKumipuyo.dir/depend:
	cd /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/test/kumipuyo /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build/test/kumipuyo /mnt/g/GoogleDrive/produce/project/BpuyoAI_sea/build/test/kumipuyo/CMakeFiles/TestKumipuyo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/kumipuyo/CMakeFiles/TestKumipuyo.dir/depend

