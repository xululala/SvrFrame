# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xululala/SvrFrame

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xululala/SvrFrame/build

# Include any dependencies generated for this target.
include CMakeFiles/primo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/primo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/primo.dir/flags.make

CMakeFiles/primo.dir/primo/log/Logger.o: CMakeFiles/primo.dir/flags.make
CMakeFiles/primo.dir/primo/log/Logger.o: ../primo/log/Logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xululala/SvrFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/primo.dir/primo/log/Logger.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/primo.dir/primo/log/Logger.o -c /home/xululala/SvrFrame/primo/log/Logger.cpp

CMakeFiles/primo.dir/primo/log/Logger.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/primo.dir/primo/log/Logger.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xululala/SvrFrame/primo/log/Logger.cpp > CMakeFiles/primo.dir/primo/log/Logger.i

CMakeFiles/primo.dir/primo/log/Logger.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/primo.dir/primo/log/Logger.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xululala/SvrFrame/primo/log/Logger.cpp -o CMakeFiles/primo.dir/primo/log/Logger.s

CMakeFiles/primo.dir/primo/util.o: CMakeFiles/primo.dir/flags.make
CMakeFiles/primo.dir/primo/util.o: ../primo/util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xululala/SvrFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/primo.dir/primo/util.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/primo.dir/primo/util.o -c /home/xululala/SvrFrame/primo/util.cpp

CMakeFiles/primo.dir/primo/util.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/primo.dir/primo/util.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xululala/SvrFrame/primo/util.cpp > CMakeFiles/primo.dir/primo/util.i

CMakeFiles/primo.dir/primo/util.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/primo.dir/primo/util.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xululala/SvrFrame/primo/util.cpp -o CMakeFiles/primo.dir/primo/util.s

CMakeFiles/primo.dir/primo/config.o: CMakeFiles/primo.dir/flags.make
CMakeFiles/primo.dir/primo/config.o: ../primo/config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xululala/SvrFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/primo.dir/primo/config.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/primo.dir/primo/config.o -c /home/xululala/SvrFrame/primo/config.cpp

CMakeFiles/primo.dir/primo/config.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/primo.dir/primo/config.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xululala/SvrFrame/primo/config.cpp > CMakeFiles/primo.dir/primo/config.i

CMakeFiles/primo.dir/primo/config.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/primo.dir/primo/config.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xululala/SvrFrame/primo/config.cpp -o CMakeFiles/primo.dir/primo/config.s

CMakeFiles/primo.dir/primo/thread.o: CMakeFiles/primo.dir/flags.make
CMakeFiles/primo.dir/primo/thread.o: ../primo/thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xululala/SvrFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/primo.dir/primo/thread.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/primo.dir/primo/thread.o -c /home/xululala/SvrFrame/primo/thread.cpp

CMakeFiles/primo.dir/primo/thread.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/primo.dir/primo/thread.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xululala/SvrFrame/primo/thread.cpp > CMakeFiles/primo.dir/primo/thread.i

CMakeFiles/primo.dir/primo/thread.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/primo.dir/primo/thread.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xululala/SvrFrame/primo/thread.cpp -o CMakeFiles/primo.dir/primo/thread.s

# Object files for target primo
primo_OBJECTS = \
"CMakeFiles/primo.dir/primo/log/Logger.o" \
"CMakeFiles/primo.dir/primo/util.o" \
"CMakeFiles/primo.dir/primo/config.o" \
"CMakeFiles/primo.dir/primo/thread.o"

# External object files for target primo
primo_EXTERNAL_OBJECTS =

../lib/libprimo.so: CMakeFiles/primo.dir/primo/log/Logger.o
../lib/libprimo.so: CMakeFiles/primo.dir/primo/util.o
../lib/libprimo.so: CMakeFiles/primo.dir/primo/config.o
../lib/libprimo.so: CMakeFiles/primo.dir/primo/thread.o
../lib/libprimo.so: CMakeFiles/primo.dir/build.make
../lib/libprimo.so: CMakeFiles/primo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xululala/SvrFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library ../lib/libprimo.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/primo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/primo.dir/build: ../lib/libprimo.so

.PHONY : CMakeFiles/primo.dir/build

CMakeFiles/primo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/primo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/primo.dir/clean

CMakeFiles/primo.dir/depend:
	cd /home/xululala/SvrFrame/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xululala/SvrFrame /home/xululala/SvrFrame /home/xululala/SvrFrame/build /home/xululala/SvrFrame/build /home/xululala/SvrFrame/build/CMakeFiles/primo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/primo.dir/depend

