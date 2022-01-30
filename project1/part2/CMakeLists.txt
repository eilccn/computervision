cmake_minimum_required(VERSION "3.17")
project(part2)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( part2 vidDisplay.cpp )
target_link_libraries( part2 ${OpenCV_LIBS} )
set( CMAKE_CXX_STANDARD 14 )


