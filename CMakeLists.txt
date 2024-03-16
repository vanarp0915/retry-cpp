# This is an example CMakeLists.txt for reference purpose to import it on your project.

cmake_minimum_required(VERSION 3.10)

project(MyProject)



include_directories(${PROJECT_SOURCE_DIR}/retry-cpp)
# Add the executable
add_executable(main main.cpp)

# Link the necessary libraries
target_link_libraries(main)
