cmake_minimum_required(VERSION 3.12)
project(cg_homework)

set(CMAKE_CXX_STANDARD 14)


find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})



set(CMAKE_CXX_FLAGS "-g -Wall")


add_executable(cg_homework main.cpp line.cpp cg.h cg.cpp circle.cpp polygon.cpp)

target_link_libraries(cg_homework ${GLUT_LIBRARY} ${OPENGL_LIBRARY})
