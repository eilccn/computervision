# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.22.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.22.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/eileenchang/computervision/project4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/eileenchang/computervision/project4/build

# Include any dependencies generated for this target.
include CMakeFiles/project4.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/project4.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/project4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/project4.dir/flags.make

CMakeFiles/project4.dir/ar_main.cpp.o: CMakeFiles/project4.dir/flags.make
CMakeFiles/project4.dir/ar_main.cpp.o: ../ar_main.cpp
CMakeFiles/project4.dir/ar_main.cpp.o: CMakeFiles/project4.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/eileenchang/computervision/project4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/project4.dir/ar_main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/project4.dir/ar_main.cpp.o -MF CMakeFiles/project4.dir/ar_main.cpp.o.d -o CMakeFiles/project4.dir/ar_main.cpp.o -c /Users/eileenchang/computervision/project4/ar_main.cpp

CMakeFiles/project4.dir/ar_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project4.dir/ar_main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/eileenchang/computervision/project4/ar_main.cpp > CMakeFiles/project4.dir/ar_main.cpp.i

CMakeFiles/project4.dir/ar_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project4.dir/ar_main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/eileenchang/computervision/project4/ar_main.cpp -o CMakeFiles/project4.dir/ar_main.cpp.s

CMakeFiles/project4.dir/functions.cpp.o: CMakeFiles/project4.dir/flags.make
CMakeFiles/project4.dir/functions.cpp.o: ../functions.cpp
CMakeFiles/project4.dir/functions.cpp.o: CMakeFiles/project4.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/eileenchang/computervision/project4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/project4.dir/functions.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/project4.dir/functions.cpp.o -MF CMakeFiles/project4.dir/functions.cpp.o.d -o CMakeFiles/project4.dir/functions.cpp.o -c /Users/eileenchang/computervision/project4/functions.cpp

CMakeFiles/project4.dir/functions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project4.dir/functions.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/eileenchang/computervision/project4/functions.cpp > CMakeFiles/project4.dir/functions.cpp.i

CMakeFiles/project4.dir/functions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project4.dir/functions.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/eileenchang/computervision/project4/functions.cpp -o CMakeFiles/project4.dir/functions.cpp.s

# Object files for target project4
project4_OBJECTS = \
"CMakeFiles/project4.dir/ar_main.cpp.o" \
"CMakeFiles/project4.dir/functions.cpp.o"

# External object files for target project4
project4_EXTERNAL_OBJECTS =

project4: CMakeFiles/project4.dir/ar_main.cpp.o
project4: CMakeFiles/project4.dir/functions.cpp.o
project4: CMakeFiles/project4.dir/build.make
project4: /usr/local/lib/libopencv_gapi.4.5.5.dylib
project4: /usr/local/lib/libopencv_stitching.4.5.5.dylib
project4: /usr/local/lib/libopencv_alphamat.4.5.5.dylib
project4: /usr/local/lib/libopencv_aruco.4.5.5.dylib
project4: /usr/local/lib/libopencv_barcode.4.5.5.dylib
project4: /usr/local/lib/libopencv_bgsegm.4.5.5.dylib
project4: /usr/local/lib/libopencv_bioinspired.4.5.5.dylib
project4: /usr/local/lib/libopencv_ccalib.4.5.5.dylib
project4: /usr/local/lib/libopencv_dnn_objdetect.4.5.5.dylib
project4: /usr/local/lib/libopencv_dnn_superres.4.5.5.dylib
project4: /usr/local/lib/libopencv_dpm.4.5.5.dylib
project4: /usr/local/lib/libopencv_face.4.5.5.dylib
project4: /usr/local/lib/libopencv_freetype.4.5.5.dylib
project4: /usr/local/lib/libopencv_fuzzy.4.5.5.dylib
project4: /usr/local/lib/libopencv_hfs.4.5.5.dylib
project4: /usr/local/lib/libopencv_img_hash.4.5.5.dylib
project4: /usr/local/lib/libopencv_intensity_transform.4.5.5.dylib
project4: /usr/local/lib/libopencv_line_descriptor.4.5.5.dylib
project4: /usr/local/lib/libopencv_mcc.4.5.5.dylib
project4: /usr/local/lib/libopencv_quality.4.5.5.dylib
project4: /usr/local/lib/libopencv_rapid.4.5.5.dylib
project4: /usr/local/lib/libopencv_reg.4.5.5.dylib
project4: /usr/local/lib/libopencv_rgbd.4.5.5.dylib
project4: /usr/local/lib/libopencv_saliency.4.5.5.dylib
project4: /usr/local/lib/libopencv_sfm.4.5.5.dylib
project4: /usr/local/lib/libopencv_stereo.4.5.5.dylib
project4: /usr/local/lib/libopencv_structured_light.4.5.5.dylib
project4: /usr/local/lib/libopencv_superres.4.5.5.dylib
project4: /usr/local/lib/libopencv_surface_matching.4.5.5.dylib
project4: /usr/local/lib/libopencv_tracking.4.5.5.dylib
project4: /usr/local/lib/libopencv_videostab.4.5.5.dylib
project4: /usr/local/lib/libopencv_viz.4.5.5.dylib
project4: /usr/local/lib/libopencv_wechat_qrcode.4.5.5.dylib
project4: /usr/local/lib/libopencv_xfeatures2d.4.5.5.dylib
project4: /usr/local/lib/libopencv_xobjdetect.4.5.5.dylib
project4: /usr/local/lib/libopencv_xphoto.4.5.5.dylib
project4: /usr/local/lib/libopencv_shape.4.5.5.dylib
project4: /usr/local/lib/libopencv_highgui.4.5.5.dylib
project4: /usr/local/lib/libopencv_datasets.4.5.5.dylib
project4: /usr/local/lib/libopencv_plot.4.5.5.dylib
project4: /usr/local/lib/libopencv_text.4.5.5.dylib
project4: /usr/local/lib/libopencv_ml.4.5.5.dylib
project4: /usr/local/lib/libopencv_phase_unwrapping.4.5.5.dylib
project4: /usr/local/lib/libopencv_optflow.4.5.5.dylib
project4: /usr/local/lib/libopencv_ximgproc.4.5.5.dylib
project4: /usr/local/lib/libopencv_video.4.5.5.dylib
project4: /usr/local/lib/libopencv_videoio.4.5.5.dylib
project4: /usr/local/lib/libopencv_imgcodecs.4.5.5.dylib
project4: /usr/local/lib/libopencv_objdetect.4.5.5.dylib
project4: /usr/local/lib/libopencv_calib3d.4.5.5.dylib
project4: /usr/local/lib/libopencv_dnn.4.5.5.dylib
project4: /usr/local/lib/libopencv_features2d.4.5.5.dylib
project4: /usr/local/lib/libopencv_flann.4.5.5.dylib
project4: /usr/local/lib/libopencv_photo.4.5.5.dylib
project4: /usr/local/lib/libopencv_imgproc.4.5.5.dylib
project4: /usr/local/lib/libopencv_core.4.5.5.dylib
project4: CMakeFiles/project4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/eileenchang/computervision/project4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable project4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/project4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/project4.dir/build: project4
.PHONY : CMakeFiles/project4.dir/build

CMakeFiles/project4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/project4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/project4.dir/clean

CMakeFiles/project4.dir/depend:
	cd /Users/eileenchang/computervision/project4/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/eileenchang/computervision/project4 /Users/eileenchang/computervision/project4 /Users/eileenchang/computervision/project4/build /Users/eileenchang/computervision/project4/build /Users/eileenchang/computervision/project4/build/CMakeFiles/project4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/project4.dir/depend

