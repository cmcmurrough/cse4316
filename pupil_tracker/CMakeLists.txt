project (pupil_tracker)
cmake_minimum_required(VERSION 2.8)

# explicitly set c++11 
set(CMAKE_CXX_STANDARD 11)

# configure OpenCV
IF(WIN32)
    # set the opencv directories manually
    message("Windows platform detected, setting OpenCV configuration manually...")
    set(OPENCV_ROOT_DIR "C:/OpenCV")
    set(OPENCV_BUILD_DIR ${OPENCV_ROOT_DIR}/build/x64/vc12)
    set(OPENCV_INCLUDE_DIRS ${OPENCV_ROOT_DIR}/build/include; ${OPENCV_ROOT_DIR}/build/include/opencv; ${OPENCV_ROOT_DIR}/build/include/opencv2)
    set(OPENCV_LIB_DIRS ${OPENCV_BUILD_DIR}/lib)

    # set the include and link directories
    include_directories(${OPENCV_INCLUDE_DIRS})
    link_directories(${OPENCV_LIB_DIRS})
    
    # find all of the libs in OPENCV_LIB_DIRS
    file(GLOB OpenCV_LIBS RELATIVE ${OPENCV_LIB_DIRS} ${OPENCV_LIB_DIRS}/*.lib)
    message("Found OpenCV libs...")
    message(${OpenCV_LIBS})
ELSE(WIN32)
    find_package(OpenCV REQUIRED)
ENDIF(WIN32)

add_executable(pupil_demo pupil_demo.cpp PupilTracker.cpp)
target_link_libraries(pupil_demo ${OpenCV_LIBS})

