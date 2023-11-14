# Set the project name.
project(Gui)

# Find Qt library.
find_package(Qt5 COMPONENTS REQUIRED Core Widgets)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})
set(INCLUDE_FILES 
	${INCLUDE_DIR}/MainWidget.hpp
	${INCLUDE_DIR}/MainControl.hpp)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(SRC_FILES 
  ${SRC_DIR}/MainGui.cpp
  ${SRC_DIR}/MainWidget.cpp
  ${SRC_DIR}/MainControl.cpp)

add_executable(${PROJECT_NAME} ${INCLUDE_FILES} ${SRC_FILES}) 

# Link libraries.
set(LIBS Core Qt5::Widgets Qt5::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})

# Include directories.
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Configure Qt moc.
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE AUTORCC TRUE)

# Add test.
set(TEST_INCLUDE_FILES 
	${INCLUDE_DIR}/MainWidget.hpp
	${INCLUDE_DIR}/MainControl.hpp)

set(TEST_SRC_FILES 
  ${SRC_DIR}/MainWidget.cpp
  ${SRC_DIR}/MainControl.cpp)

add_executable(gui_test test/test.cpp ${TEST_INCLUDE_FILES} ${TEST_SRC_FILES})
target_include_directories(gui_test PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(gui_test PUBLIC ${LIBS} PRIVATE GTest::gtest_main)
set_target_properties(gui_test PROPERTIES AUTOMOC TRUE AUTORCC TRUE)
