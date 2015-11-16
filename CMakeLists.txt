cmake_minimum_required(VERSION 2.8.9)
project(softbodies)
include_directories(include)
file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(run ${SOURCES})

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
target_link_libraries(run ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} dl)

file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})

add_definitions(-DDEBUG_GL)

target_compile_features(run PRIVATE cxx_range_for)
