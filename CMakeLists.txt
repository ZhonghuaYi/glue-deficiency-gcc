﻿cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-std=c++17)
set(CMAKE_CXX_FLAGS "-std=c++17")
project(glue_deficiency_gcc)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_EXE_LINKER_FLAGS "-lcblas")
endif()

find_package(OpenCV REQUIRED)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
aux_source_directory(src SRC_LIST)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(glue_deficiency_gcc ${SRC_LIST} include/data.h src/data.cpp)
target_link_libraries(glue_deficiency_gcc ${OpenCV_LIBS})