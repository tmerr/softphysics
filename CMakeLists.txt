cmake_minimum_required(VERSION 3.1)
project(softbodies)

set (CMAKE_CXX_STANDARD 11)
add_definitions(-DDEBUG_GL)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
include_directories("src")
include_directories(include)

file(GLOB SOURCES "src/*.cpp" "src/*.c")
file(GLOB TEST_SOURCES "src/*.cpp" "src/*.c" "tests/test.cpp")
list(REMOVE_ITEM TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable(run ${SOURCES})
target_link_libraries(run ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} dl)

add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} dl)
add_custom_command(TARGET unit_tests POST_BUILD COMMAND unit_tests)

file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})
