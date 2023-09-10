cmake_minimum_required(VERSION 3.23)

# Set the project name.
project(Core)

# Set C++ standard.
set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})
set(INCLUDE_FILES 
	${INCLUDE_DIR}/Core.hpp)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(SRC_FILES 
	${SRC_DIR}/Core.cpp)

add_library(${PROJECT_NAME} STATIC ${INCLUDE_FILES} ${SRC_FILES}) 

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Add test.
add_executable(test test/test.cpp)
target_link_libraries(test PRIVATE ${PROJECT_NAME} GTest::gtest_main)