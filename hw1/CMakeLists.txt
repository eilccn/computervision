cmake_minimum_required(VERSION "3.17")
project(hw1)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( hw1 pow.cpp )
target_link_libraries( hw1 ${OpenCV_LIBS} )
set( CMAKE_CXX_STANDARD 14 )