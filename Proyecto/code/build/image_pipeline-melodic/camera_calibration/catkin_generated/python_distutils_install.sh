#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/doblas/TFG/Proyecto/code/src/image_pipeline-melodic/camera_calibration"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/doblas/TFG/Proyecto/code/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/doblas/TFG/Proyecto/code/install/lib/python2.7/dist-packages:/home/doblas/TFG/Proyecto/code/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/doblas/TFG/Proyecto/code/build" \
    "/usr/bin/python2" \
    "/home/doblas/TFG/Proyecto/code/src/image_pipeline-melodic/camera_calibration/setup.py" \
     \
    build --build-base "/home/doblas/TFG/Proyecto/code/build/image_pipeline-melodic/camera_calibration" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/doblas/TFG/Proyecto/code/install" --install-scripts="/home/doblas/TFG/Proyecto/code/install/bin"
