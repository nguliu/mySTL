cmake_minimum_required(VERSION 3.10.2)	#如果cmake版本过低请自行更改

project(mySTL CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CXX_FLAGS
    -g
#   -Wall
#	-Werror
    -std=c++11
    -D_PTHREADS
    -Wno-unused-parameter
)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

#设置可执行文件输出在工程目录上两级的bin目录下
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../bin)

#设置库文件输出在工程目录的lib目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#注意要包含源文件目录，否则程序找不到库文件
include_directories(${PROJECT_SOURCE_DIR})

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})


add_subdirectory(mySTL)
add_subdirectory(tests)