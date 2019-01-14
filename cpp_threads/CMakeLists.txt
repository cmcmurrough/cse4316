project (threads)
cmake_minimum_required(VERSION 3.1)

# explicitly set c++11 
set(CMAKE_CXX_STANDARD 11)

# find thread packages
find_package(Threads)

# find boost
find_package(Boost 1.55.0 REQUIRED system thread)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
IF(WIN32)
	set(Boost_USE_STATIC_LIBS OFF)
	set(Boost_USE_MULTITHREADED ON)
	set(Boost_USE_STATIC_RUNTIME OFF)
	set(BOOST_ALL_DYN_LINK ON)
ENDIF(WIN32)

# create individual projects
add_executable(serial_polling serial_polling.cpp)
target_link_libraries(serial_polling ${Boost_LIBRARIES})


