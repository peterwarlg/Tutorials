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
CMAKE_SOURCE_DIR = /tmp/tmp.GCzzN1G1R0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.GCzzN1G1R0/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/shared_memory_for_read_process.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/shared_memory_for_read_process.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/shared_memory_for_read_process.dir/flags.make

CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.o: CMakeFiles/shared_memory_for_read_process.dir/flags.make
CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.o: ../shm_write.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.GCzzN1G1R0/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.o -c /tmp/tmp.GCzzN1G1R0/shm_write.cpp

CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.GCzzN1G1R0/shm_write.cpp > CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.i

CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.GCzzN1G1R0/shm_write.cpp -o CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.s

# Object files for target shared_memory_for_read_process
shared_memory_for_read_process_OBJECTS = \
"CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.o"

# External object files for target shared_memory_for_read_process
shared_memory_for_read_process_EXTERNAL_OBJECTS =

shared_memory_for_read_process: CMakeFiles/shared_memory_for_read_process.dir/shm_write.cpp.o
shared_memory_for_read_process: CMakeFiles/shared_memory_for_read_process.dir/build.make
shared_memory_for_read_process: CMakeFiles/shared_memory_for_read_process.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.GCzzN1G1R0/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable shared_memory_for_read_process"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shared_memory_for_read_process.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/shared_memory_for_read_process.dir/build: shared_memory_for_read_process

.PHONY : CMakeFiles/shared_memory_for_read_process.dir/build

CMakeFiles/shared_memory_for_read_process.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/shared_memory_for_read_process.dir/cmake_clean.cmake
.PHONY : CMakeFiles/shared_memory_for_read_process.dir/clean

CMakeFiles/shared_memory_for_read_process.dir/depend:
	cd /tmp/tmp.GCzzN1G1R0/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.GCzzN1G1R0 /tmp/tmp.GCzzN1G1R0 /tmp/tmp.GCzzN1G1R0/cmake-build-debug /tmp/tmp.GCzzN1G1R0/cmake-build-debug /tmp/tmp.GCzzN1G1R0/cmake-build-debug/CMakeFiles/shared_memory_for_read_process.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/shared_memory_for_read_process.dir/depend
