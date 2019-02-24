cmake_minimum_required(VERSION 3.5)

#find_package(glfw3 REQUIRED)
#find_package(OpenGL REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main glfw GLEW GL)
