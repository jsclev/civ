cmake_minimum_required(VERSION 3.14)
project(civ)

set(CMAKE_CXX_STANDARD 14)

set(SDL2_INCLUDE_DIR /usr/local/Cellar/sdl2/2.0.9/include/SDL2)
set(SDL2_IMAGE_INCLUDE_DIR /usr/local/Cellar/sdl2_image/2.0.4/include/SDL2)
set(SDL2_TTF_INCLUDE_DIR /usr/local/Cellar/sdl2_ttf/2.0.14/include/SDL2)

set(SDL2_LIB_DIR /usr/local/Cellar/sdl2/2.0.9/lib)
set(SDL2_IMAGE_LIB_DIR /usr/local/Cellar/sdl2_image/2.0.4/lib)
set(SDL2_TTF_LIB_DIR /usr/local/Cellar/sdl2_ttf/2.0.14/lib)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR} ${SDL2_IMAGE_LIB_DIR} ${SDL2_TTF_LIB_DIR})

add_executable(civ src/main.cpp src/engine/engine.cpp src/engine/engine.h src/engine/Tile.cpp src/engine/Tile.h src/engine/sdl_utils.cpp src/engine/sdl_utils.h src/engine/LTexture.cpp src/engine/LTexture.h src/engine/Dot.cpp src/engine/Dot.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_image SDL2_ttf)