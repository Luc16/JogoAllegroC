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
include addons/font/CMakeFiles/allegro_font.dir/depend.make

# Include the progress variables for this target.
include addons/font/CMakeFiles/allegro_font.dir/progress.make

# Include the compile flags for this target's objects.
include addons/font/CMakeFiles/allegro_font.dir/flags.make

addons/font/CMakeFiles/allegro_font.dir/font.c.o: addons/font/CMakeFiles/allegro_font.dir/flags.make
addons/font/CMakeFiles/allegro_font.dir/font.c.o: ../addons/font/font.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariana/Desktop/allegro/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object addons/font/CMakeFiles/allegro_font.dir/font.c.o"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/allegro_font.dir/font.c.o   -c /Users/mariana/Desktop/allegro/addons/font/font.c

addons/font/CMakeFiles/allegro_font.dir/font.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/allegro_font.dir/font.c.i"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mariana/Desktop/allegro/addons/font/font.c > CMakeFiles/allegro_font.dir/font.c.i

addons/font/CMakeFiles/allegro_font.dir/font.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/allegro_font.dir/font.c.s"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mariana/Desktop/allegro/addons/font/font.c -o CMakeFiles/allegro_font.dir/font.c.s

addons/font/CMakeFiles/allegro_font.dir/fontbmp.c.o: addons/font/CMakeFiles/allegro_font.dir/flags.make
addons/font/CMakeFiles/allegro_font.dir/fontbmp.c.o: ../addons/font/fontbmp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariana/Desktop/allegro/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object addons/font/CMakeFiles/allegro_font.dir/fontbmp.c.o"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/allegro_font.dir/fontbmp.c.o   -c /Users/mariana/Desktop/allegro/addons/font/fontbmp.c

addons/font/CMakeFiles/allegro_font.dir/fontbmp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/allegro_font.dir/fontbmp.c.i"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mariana/Desktop/allegro/addons/font/fontbmp.c > CMakeFiles/allegro_font.dir/fontbmp.c.i

addons/font/CMakeFiles/allegro_font.dir/fontbmp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/allegro_font.dir/fontbmp.c.s"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mariana/Desktop/allegro/addons/font/fontbmp.c -o CMakeFiles/allegro_font.dir/fontbmp.c.s

addons/font/CMakeFiles/allegro_font.dir/stdfont.c.o: addons/font/CMakeFiles/allegro_font.dir/flags.make
addons/font/CMakeFiles/allegro_font.dir/stdfont.c.o: ../addons/font/stdfont.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariana/Desktop/allegro/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object addons/font/CMakeFiles/allegro_font.dir/stdfont.c.o"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/allegro_font.dir/stdfont.c.o   -c /Users/mariana/Desktop/allegro/addons/font/stdfont.c

addons/font/CMakeFiles/allegro_font.dir/stdfont.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/allegro_font.dir/stdfont.c.i"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mariana/Desktop/allegro/addons/font/stdfont.c > CMakeFiles/allegro_font.dir/stdfont.c.i

addons/font/CMakeFiles/allegro_font.dir/stdfont.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/allegro_font.dir/stdfont.c.s"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mariana/Desktop/allegro/addons/font/stdfont.c -o CMakeFiles/allegro_font.dir/stdfont.c.s

addons/font/CMakeFiles/allegro_font.dir/text.c.o: addons/font/CMakeFiles/allegro_font.dir/flags.make
addons/font/CMakeFiles/allegro_font.dir/text.c.o: ../addons/font/text.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariana/Desktop/allegro/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object addons/font/CMakeFiles/allegro_font.dir/text.c.o"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/allegro_font.dir/text.c.o   -c /Users/mariana/Desktop/allegro/addons/font/text.c

addons/font/CMakeFiles/allegro_font.dir/text.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/allegro_font.dir/text.c.i"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mariana/Desktop/allegro/addons/font/text.c > CMakeFiles/allegro_font.dir/text.c.i

addons/font/CMakeFiles/allegro_font.dir/text.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/allegro_font.dir/text.c.s"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mariana/Desktop/allegro/addons/font/text.c -o CMakeFiles/allegro_font.dir/text.c.s

addons/font/CMakeFiles/allegro_font.dir/bmfont.c.o: addons/font/CMakeFiles/allegro_font.dir/flags.make
addons/font/CMakeFiles/allegro_font.dir/bmfont.c.o: ../addons/font/bmfont.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariana/Desktop/allegro/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object addons/font/CMakeFiles/allegro_font.dir/bmfont.c.o"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/allegro_font.dir/bmfont.c.o   -c /Users/mariana/Desktop/allegro/addons/font/bmfont.c

addons/font/CMakeFiles/allegro_font.dir/bmfont.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/allegro_font.dir/bmfont.c.i"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mariana/Desktop/allegro/addons/font/bmfont.c > CMakeFiles/allegro_font.dir/bmfont.c.i

addons/font/CMakeFiles/allegro_font.dir/bmfont.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/allegro_font.dir/bmfont.c.s"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mariana/Desktop/allegro/addons/font/bmfont.c -o CMakeFiles/allegro_font.dir/bmfont.c.s

addons/font/CMakeFiles/allegro_font.dir/xml.c.o: addons/font/CMakeFiles/allegro_font.dir/flags.make
addons/font/CMakeFiles/allegro_font.dir/xml.c.o: ../addons/font/xml.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariana/Desktop/allegro/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object addons/font/CMakeFiles/allegro_font.dir/xml.c.o"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/allegro_font.dir/xml.c.o   -c /Users/mariana/Desktop/allegro/addons/font/xml.c

addons/font/CMakeFiles/allegro_font.dir/xml.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/allegro_font.dir/xml.c.i"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mariana/Desktop/allegro/addons/font/xml.c > CMakeFiles/allegro_font.dir/xml.c.i

addons/font/CMakeFiles/allegro_font.dir/xml.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/allegro_font.dir/xml.c.s"
	cd /Users/mariana/Desktop/allegro/build/addons/font && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mariana/Desktop/allegro/addons/font/xml.c -o CMakeFiles/allegro_font.dir/xml.c.s

# Object files for target allegro_font
allegro_font_OBJECTS = \
"CMakeFiles/allegro_font.dir/font.c.o" \
"CMakeFiles/allegro_font.dir/fontbmp.c.o" \
"CMakeFiles/allegro_font.dir/stdfont.c.o" \
"CMakeFiles/allegro_font.dir/text.c.o" \
"CMakeFiles/allegro_font.dir/bmfont.c.o" \
"CMakeFiles/allegro_font.dir/xml.c.o"

# External object files for target allegro_font
allegro_font_EXTERNAL_OBJECTS =

lib/liballegro_font.5.2.6.dylib: addons/font/CMakeFiles/allegro_font.dir/font.c.o
lib/liballegro_font.5.2.6.dylib: addons/font/CMakeFiles/allegro_font.dir/fontbmp.c.o
lib/liballegro_font.5.2.6.dylib: addons/font/CMakeFiles/allegro_font.dir/stdfont.c.o
lib/liballegro_font.5.2.6.dylib: addons/font/CMakeFiles/allegro_font.dir/text.c.o
lib/liballegro_font.5.2.6.dylib: addons/font/CMakeFiles/allegro_font.dir/bmfont.c.o
lib/liballegro_font.5.2.6.dylib: addons/font/CMakeFiles/allegro_font.dir/xml.c.o
lib/liballegro_font.5.2.6.dylib: addons/font/CMakeFiles/allegro_font.dir/build.make
lib/liballegro_font.5.2.6.dylib: lib/liballegro.5.2.6.dylib
lib/liballegro_font.5.2.6.dylib: addons/font/CMakeFiles/allegro_font.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/mariana/Desktop/allegro/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking C shared library ../../lib/liballegro_font.dylib"
	cd /Users/mariana/Desktop/allegro/build/addons/font && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/allegro_font.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/mariana/Desktop/allegro/build/addons/font && $(CMAKE_COMMAND) -E cmake_symlink_library ../../lib/liballegro_font.5.2.6.dylib ../../lib/liballegro_font.5.2.dylib ../../lib/liballegro_font.dylib

lib/liballegro_font.5.2.dylib: lib/liballegro_font.5.2.6.dylib
	@$(CMAKE_COMMAND) -E touch_nocreate lib/liballegro_font.5.2.dylib

lib/liballegro_font.dylib: lib/liballegro_font.5.2.6.dylib
	@$(CMAKE_COMMAND) -E touch_nocreate lib/liballegro_font.dylib

# Rule to build all files generated by this target.
addons/font/CMakeFiles/allegro_font.dir/build: lib/liballegro_font.dylib

.PHONY : addons/font/CMakeFiles/allegro_font.dir/build

addons/font/CMakeFiles/allegro_font.dir/clean:
	cd /Users/mariana/Desktop/allegro/build/addons/font && $(CMAKE_COMMAND) -P CMakeFiles/allegro_font.dir/cmake_clean.cmake
.PHONY : addons/font/CMakeFiles/allegro_font.dir/clean

addons/font/CMakeFiles/allegro_font.dir/depend:
	cd /Users/mariana/Desktop/allegro/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mariana/Desktop/allegro /Users/mariana/Desktop/allegro/addons/font /Users/mariana/Desktop/allegro/build /Users/mariana/Desktop/allegro/build/addons/font /Users/mariana/Desktop/allegro/build/addons/font/CMakeFiles/allegro_font.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : addons/font/CMakeFiles/allegro_font.dir/depend

