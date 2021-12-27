cmake_minimum_required(VERSION 3.17)
project(glue_deficiency_gcc)
set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
aux_source_directory(src SRC_LIST)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(glue_deficiency_gcc ${SRC_LIST})
target_link_libraries(glue_deficiency_gcc ${OpenCV_LIBS})