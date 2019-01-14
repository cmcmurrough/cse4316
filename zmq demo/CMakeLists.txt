project (zeromq_example)
cmake_minimum_required(VERSION 2.8)

# configure ZeroMQ
IF(WIN32)
    # set the zeromq directories manually
    message("Windows platform detected, setting ZeroMQ configuration manually...")
    set(ZeroMQ_ROOT_DIR "C:/Program Files/ZeroMQ 4.0.4")
    set(ZeroMQ_INCLUDE_DIR ${ZeroMQ_ROOT_DIR}/include)
    set(ZeroMQ_LIB_DIR ${ZeroMQ_ROOT_DIR}/lib)

    # set the include and link directories
    include_directories(${ZeroMQ_INCLUDE_DIR})
    link_directories(${ZeroMQ_LIB_DIR})
    
    # find all of the libs in ZEROMQ_LIB_DIR
    file(GLOB ZeroMQ_LIBS RELATIVE ${ZeroMQ_LIB_DIR} ${ZeroMQ_LIB_DIR}/*.lib)
    message("Found ZeroMQ libs...")
    message(${ZeroMQ_LIBS})
ELSE(WIN32)
    #find_package (ZeroMQ REQUIRED)
	#set(ZeroMQ_LIBS ${ZeroMQ_LIBRARIES})
 
    # set the zeromq directories
    message("Linux platform detected, setting ZeroMQ configuration manually...")
    find_path(ZeroMQ_ROOT_DIR NAMES include/zmq.h)
    find_library(ZeroMQ_LIBS NAMES zmq libzmq HINTS ${ZeroMQ_ROOT_DIR}/lib)
    find_path(ZeroMQ_INCLUDE_DIR NAMES zmq.h HINTS ${ZeroMQ_ROOT_DIR}/include)
    
    # set the include and link directories
    include_directories(${ZeroMQ_INCLUDE_DIR})
    link_directories(${ZeroMQ_LIB_DIR})
ENDIF(WIN32)

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
    find_package (OpenCV REQUIRED)
ENDIF(WIN32)

# create individual projects
add_executable (zmq_cv_request zmq_cv_request.cpp)
target_link_libraries(zmq_cv_request ${ZeroMQ_LIBS} ${OpenCV_LIBS})

add_executable (zmq_cv_server zmq_cv_server.cpp)
target_link_libraries(zmq_cv_server ${ZeroMQ_LIBS} ${OpenCV_LIBS})
