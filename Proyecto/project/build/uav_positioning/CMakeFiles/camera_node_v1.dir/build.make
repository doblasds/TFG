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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/doblas/TFG/Proyecto/project/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/doblas/TFG/Proyecto/project/build

# Include any dependencies generated for this target.
include uav_positioning/CMakeFiles/camera_node_v1.dir/depend.make

# Include the progress variables for this target.
include uav_positioning/CMakeFiles/camera_node_v1.dir/progress.make

# Include the compile flags for this target's objects.
include uav_positioning/CMakeFiles/camera_node_v1.dir/flags.make

uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o: uav_positioning/CMakeFiles/camera_node_v1.dir/flags.make
uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o: /home/doblas/TFG/Proyecto/project/src/uav_positioning/src/test_versions/camera_node_v1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/doblas/TFG/Proyecto/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o"
	cd /home/doblas/TFG/Proyecto/project/build/uav_positioning && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o -c /home/doblas/TFG/Proyecto/project/src/uav_positioning/src/test_versions/camera_node_v1.cpp

uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.i"
	cd /home/doblas/TFG/Proyecto/project/build/uav_positioning && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/doblas/TFG/Proyecto/project/src/uav_positioning/src/test_versions/camera_node_v1.cpp > CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.i

uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.s"
	cd /home/doblas/TFG/Proyecto/project/build/uav_positioning && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/doblas/TFG/Proyecto/project/src/uav_positioning/src/test_versions/camera_node_v1.cpp -o CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.s

uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o.requires:

.PHONY : uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o.requires

uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o.provides: uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o.requires
	$(MAKE) -f uav_positioning/CMakeFiles/camera_node_v1.dir/build.make uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o.provides.build
.PHONY : uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o.provides

uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o.provides.build: uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o


# Object files for target camera_node_v1
camera_node_v1_OBJECTS = \
"CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o"

# External object files for target camera_node_v1
camera_node_v1_EXTERNAL_OBJECTS =

/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: uav_positioning/CMakeFiles/camera_node_v1.dir/build.make
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libroscpp.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/librosconsole.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/librosconsole_log4cxx.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/librosconsole_backend_interface.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/liblog4cxx.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_regex.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libxmlrpcpp.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libroscpp_serialization.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/librostime.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libcpp_common.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_system.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_thread.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so.0.4
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libimage_transport.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libmessage_filters.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libclass_loader.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/libPocoFoundation.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libdl.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libroslib.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/librospack.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libpython2.7.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libtinyxml2.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libcamera_info_manager.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libcamera_calibration_parsers.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libroscpp.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/librosconsole.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/librosconsole_log4cxx.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/librosconsole_backend_interface.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/liblog4cxx.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_regex.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libxmlrpcpp.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libroscpp_serialization.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/librostime.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /opt/ros/melodic/lib/libcpp_common.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_system.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_thread.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so.0.4
/home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1: uav_positioning/CMakeFiles/camera_node_v1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/doblas/TFG/Proyecto/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1"
	cd /home/doblas/TFG/Proyecto/project/build/uav_positioning && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/camera_node_v1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
uav_positioning/CMakeFiles/camera_node_v1.dir/build: /home/doblas/TFG/Proyecto/project/devel/lib/uav_positioning/camera_node_v1

.PHONY : uav_positioning/CMakeFiles/camera_node_v1.dir/build

uav_positioning/CMakeFiles/camera_node_v1.dir/requires: uav_positioning/CMakeFiles/camera_node_v1.dir/src/test_versions/camera_node_v1.cpp.o.requires

.PHONY : uav_positioning/CMakeFiles/camera_node_v1.dir/requires

uav_positioning/CMakeFiles/camera_node_v1.dir/clean:
	cd /home/doblas/TFG/Proyecto/project/build/uav_positioning && $(CMAKE_COMMAND) -P CMakeFiles/camera_node_v1.dir/cmake_clean.cmake
.PHONY : uav_positioning/CMakeFiles/camera_node_v1.dir/clean

uav_positioning/CMakeFiles/camera_node_v1.dir/depend:
	cd /home/doblas/TFG/Proyecto/project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/doblas/TFG/Proyecto/project/src /home/doblas/TFG/Proyecto/project/src/uav_positioning /home/doblas/TFG/Proyecto/project/build /home/doblas/TFG/Proyecto/project/build/uav_positioning /home/doblas/TFG/Proyecto/project/build/uav_positioning/CMakeFiles/camera_node_v1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : uav_positioning/CMakeFiles/camera_node_v1.dir/depend

