cmake_minimum_required(VERSION 3.5)
project(ece-6122-final-project)

set(CMAKE_CXX_STANDARD 11)

# Set some compiler flags (These assume GNU C++ compiler!)
set (CMAKE_CXX_FLAGS "-Wall -Wextra" CACHE STRING "" FORCE)
set (CMAKE_C_FLAGS "-Wall -Wextra" CACHE STRING "" FORCE)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")

# Find the standard libraries that were installed with apt-get

# sudo apt-get install libglew
find_package (GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# sudo apt-get install libgl1-mesa-dev
find_package (OpenGL REQUIRED)
include_directories (${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

# sudo apt-get install libbullet-dev
find_package(Bullet REQUIRED)
include_directories(${BULLET_INCLUDE_DIRS})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

# Set the CMAKE_MODULE_PATH to the cmake-modules/ directory.
# This directory contains custom *.cmake files we need for
# compiling things outside the normal reach of cmake.
# Read below for more details.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)

# sudo apt-get install libglfw3-dev
# NOTE: GLFW does not actually provide a FindGLFW3.cmake file!
# To workaround this there's a custom FindGLFW3.cmake in the
# cmake-modules/ directory. CMAKE_MODULE_PATH must include the
# cmake-modules/ directory before looking for GLFW3
find_package (GLFW3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIRS})
link_libraries(${GLFW3_LIBRARIES})

# sudo apt-get install libfreeimage-dev
# NOTE: Same as with GLFW, libfreeimage-dev does not come with a
# FindFreeImage.cmake file. Thus, we have to look for it after
# setting the CMAKE_MODULE_PATH to include the cmake-modules/ directory
# directory
find_package (FreeImage REQUIRED)
include_directories (${FREEIMAGE_INCLUDE_DIRS})
link_libraries(${FREEIMAGE_LIBRARIES})

# Gather up all the headers and sources in one variable: SOURCES
set (SRC_DIR "src")
file (GLOB SOURCES ${SRC_DIR}/*.c ${SRC_DIR}/*.cpp ${SRC_DIR}/*.h)

add_executable(${PROJECT_NAME} ${SOURCES}) 

target_link_libraries(${PROJECT_NAME}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLFW3_LIBRARIES}
    ${FREEIMAGE_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${FREETYPE_LIBRARIES}
)
