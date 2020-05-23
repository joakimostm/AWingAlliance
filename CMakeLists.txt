project(AWingAlliance)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)

# find_path : finds header files
# find_library : finds libraries
# find_package : finds some predefined file that does a bunch of the two above


find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")


include_directories(    ${PROJECT_SOURCE_DIR}/include
                        ${PROJECT_SOURCE_DIR}/include/external
${GLM_INCLUDE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/src)


# add_library(mylib SHARED IMPORTED)
# set_property(TARGET mylib
#               PROPERTY IMPORTED_LOCATION
#               "${LIBRARY_OUTPUT_PATH}/mylib.so")
