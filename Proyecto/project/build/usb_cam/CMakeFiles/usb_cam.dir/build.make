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
include usb_cam/CMakeFiles/usb_cam.dir/depend.make

# Include the progress variables for this target.
include usb_cam/CMakeFiles/usb_cam.dir/progress.make

# Include the compile flags for this target's objects.
include usb_cam/CMakeFiles/usb_cam.dir/flags.make

usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o: usb_cam/CMakeFiles/usb_cam.dir/flags.make
usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o: /home/doblas/TFG/Proyecto/project/src/usb_cam/src/usb_cam.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/doblas/TFG/Proyecto/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o"
	cd /home/doblas/TFG/Proyecto/project/build/usb_cam && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o -c /home/doblas/TFG/Proyecto/project/src/usb_cam/src/usb_cam.cpp

usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/usb_cam.dir/src/usb_cam.cpp.i"
	cd /home/doblas/TFG/Proyecto/project/build/usb_cam && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/doblas/TFG/Proyecto/project/src/usb_cam/src/usb_cam.cpp > CMakeFiles/usb_cam.dir/src/usb_cam.cpp.i

usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/usb_cam.dir/src/usb_cam.cpp.s"
	cd /home/doblas/TFG/Proyecto/project/build/usb_cam && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/doblas/TFG/Proyecto/project/src/usb_cam/src/usb_cam.cpp -o CMakeFiles/usb_cam.dir/src/usb_cam.cpp.s

usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o.requires:

.PHONY : usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o.requires

usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o.provides: usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o.requires
	$(MAKE) -f usb_cam/CMakeFiles/usb_cam.dir/build.make usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o.provides.build
.PHONY : usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o.provides

usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o.provides.build: usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o


# Object files for target usb_cam
usb_cam_OBJECTS = \
"CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o"

# External object files for target usb_cam
usb_cam_EXTERNAL_OBJECTS =

/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: usb_cam/CMakeFiles/usb_cam.dir/build.make
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/libimage_transport.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/libmessage_filters.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/libclass_loader.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/libPocoFoundation.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libdl.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/libroslib.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/librospack.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libpython2.7.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libtinyxml2.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/libcamera_info_manager.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/libcamera_calibration_parsers.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/libroscpp.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/librosconsole.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/librosconsole_log4cxx.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/librosconsole_backend_interface.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/liblog4cxx.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libboost_regex.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/libxmlrpcpp.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/libroscpp_serialization.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/librostime.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /opt/ros/melodic/lib/libcpp_common.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so.0.4
/home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so: usb_cam/CMakeFiles/usb_cam.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/doblas/TFG/Proyecto/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library /home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so"
	cd /home/doblas/TFG/Proyecto/project/build/usb_cam && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/usb_cam.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
usb_cam/CMakeFiles/usb_cam.dir/build: /home/doblas/TFG/Proyecto/project/devel/lib/libusb_cam.so

.PHONY : usb_cam/CMakeFiles/usb_cam.dir/build

usb_cam/CMakeFiles/usb_cam.dir/requires: usb_cam/CMakeFiles/usb_cam.dir/src/usb_cam.cpp.o.requires

.PHONY : usb_cam/CMakeFiles/usb_cam.dir/requires

usb_cam/CMakeFiles/usb_cam.dir/clean:
	cd /home/doblas/TFG/Proyecto/project/build/usb_cam && $(CMAKE_COMMAND) -P CMakeFiles/usb_cam.dir/cmake_clean.cmake
.PHONY : usb_cam/CMakeFiles/usb_cam.dir/clean

usb_cam/CMakeFiles/usb_cam.dir/depend:
	cd /home/doblas/TFG/Proyecto/project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/doblas/TFG/Proyecto/project/src /home/doblas/TFG/Proyecto/project/src/usb_cam /home/doblas/TFG/Proyecto/project/build /home/doblas/TFG/Proyecto/project/build/usb_cam /home/doblas/TFG/Proyecto/project/build/usb_cam/CMakeFiles/usb_cam.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : usb_cam/CMakeFiles/usb_cam.dir/depend

