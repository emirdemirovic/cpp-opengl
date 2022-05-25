cmake_minimum_required(VERSION 3.16)  
project(opengl)               
set(CMAKE_CXX_STANDARD 14)  

set(SOURCE_FILES main.cpp opengl.cpp tgaimage.cpp model.cpp)
add_executable(opengl ${SOURCE_FILES})