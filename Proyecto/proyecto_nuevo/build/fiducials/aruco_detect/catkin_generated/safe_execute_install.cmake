execute_process(COMMAND "/home/doblas/TFG/Proyecto/proyecto_nuevo/build/fiducials/aruco_detect/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/doblas/TFG/Proyecto/proyecto_nuevo/build/fiducials/aruco_detect/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
