# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.17.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.17.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mariana/Desktop/allegro

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mariana/Desktop/allegro/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/ex_nodisplay.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/ex_nodisplay.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/ex_nodisplay.dir/flags.make

examples/CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.o: examples/CMakeFiles/ex_nodisplay.dir/flags.make
examples/CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.o: ../examples/ex_nodisplay.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariana/Desktop/allegro/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.o"
	cd /Users/mariana/Desktop/allegro/build/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.o   -c /Users/mariana/Desktop/allegro/examples/ex_nodisplay.c

examples/CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.i"
	cd /Users/mariana/Desktop/allegro/build/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mariana/Desktop/allegro/examples/ex_nodisplay.c > CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.i

examples/CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.s"
	cd /Users/mariana/Desktop/allegro/build/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mariana/Desktop/allegro/examples/ex_nodisplay.c -o CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.s

# Object files for target ex_nodisplay
ex_nodisplay_OBJECTS = \
"CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.o"

# External object files for target ex_nodisplay
ex_nodisplay_EXTERNAL_OBJECTS =

examples/ex_nodisplay: examples/CMakeFiles/ex_nodisplay.dir/ex_nodisplay.c.o
examples/ex_nodisplay: examples/CMakeFiles/ex_nodisplay.dir/build.make
examples/ex_nodisplay: lib/liballegro_main.5.2.6.dylib
examples/ex_nodisplay: lib/liballegro_image.5.2.6.dylib
examples/ex_nodisplay: lib/liballegro_dialog.5.2.6.dylib
examples/ex_nodisplay: /usr/local/Cellar/webp/1.1.0/lib/libwebp.dylib
examples/ex_nodisplay: lib/liballegro.5.2.6.dylib
examples/ex_nodisplay: examples/CMakeFiles/ex_nodisplay.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/mariana/Desktop/allegro/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ex_nodisplay"
	cd /Users/mariana/Desktop/allegro/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ex_nodisplay.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/ex_nodisplay.dir/build: examples/ex_nodisplay

.PHONY : examples/CMakeFiles/ex_nodisplay.dir/build

examples/CMakeFiles/ex_nodisplay.dir/clean:
	cd /Users/mariana/Desktop/allegro/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/ex_nodisplay.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/ex_nodisplay.dir/clean

examples/CMakeFiles/ex_nodisplay.dir/depend:
	cd /Users/mariana/Desktop/allegro/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mariana/Desktop/allegro /Users/mariana/Desktop/allegro/examples /Users/mariana/Desktop/allegro/build /Users/mariana/Desktop/allegro/build/examples /Users/mariana/Desktop/allegro/build/examples/CMakeFiles/ex_nodisplay.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/ex_nodisplay.dir/depend

