project (cv_capture_example)
cmake_minimum_required(VERSION 2.8)

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
    # workaround for -lopencv_dep_cudart linker error when using CUDA
    # NOTE: if linker error above occurs, run cmake again with existing cache before build
    set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
    
    set(OpenCV_DIR "/usr/local/share/OpenCV")
    find_package(OpenCV REQUIRED)
ENDIF(WIN32)

# create create individual projects
add_executable (cv_capture_example cv_capture_example.cpp)
target_link_libraries(cv_capture_example ${OpenCV_LIBS})

add_executable (cv_color_detector cv_color_detector.cpp)
target_link_libraries(cv_color_detector ${OpenCV_LIBS})
