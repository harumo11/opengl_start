cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_GLAGS "-Wall -g")

add_executable(main main.cpp)
target_link_libraries(main glfw GLEW GL)
