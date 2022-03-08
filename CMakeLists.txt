cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)
#add_definitions(-std=c++17)
project(glue_deficiency_gcc)

find_package(OpenCV REQUIRED)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
aux_source_directory(src SRC_LIST)
include_directories(${PROJECT_SOURCE_DIR}/include)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(glue_deficiency_gcc ${SRC_LIST})
target_link_libraries(glue_deficiency_gcc ${OpenCV_LIBS})