cmake_minimum_required(VERSION 3.23)

# Set the project name.
project(ComputerVisionApp)

# Set C++ standard.
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add main modules.
add_subdirectory(${PROJECT_SOURCE_DIR}/sources/Core)
add_subdirectory(${PROJECT_SOURCE_DIR}/sources/Gui)
