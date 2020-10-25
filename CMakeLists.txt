cmake_minimum_required(VERSION 3.14)
project(civ)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(civ src/main.cpp)

include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::TTF)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
