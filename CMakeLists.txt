cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Find SDL
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src)

# Find SDL2_ttf
# SDL2_ttf manual include and library
# find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h PATH_SUFFIXES include PATHS opt/homebrew)
# find_package(SDL2_TTF REQUIRED)
find_library(SDL2_TTF_LIBRARY SDL2_ttf)
set(SDL2_TTF_INCLUDE_DIR /opt/homebrew/Cellar/sdl2_ttf/2.24.0/include/SDL2)

message(STATUS "SDL2_ttf include dir: ${SDL2_TTF_INCLUDE_DIR}")  #/opt/homebrew/Cellar/sdl2_ttf/2.24.0/include/SDL2/SDL_ttf.h
message(STATUS "SDL2_ttf library    : ${SDL2_TTF_LIBRARY} ${SDL2_LIBRARIES}")

include_directories(${SDL2_TTF_INCLUDE_DIR})

# target
add_executable(SnakeGame src/main.cpp src/game.cpp src/controller.cpp 
                        src/renderer.cpp src/snake.cpp src/food.cpp 
                        src/scoreio.cpp src/menu.cpp src/enemy/planner.cpp)

# link
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
string(STRIP ${SDL2_TTF_LIBRARY} SDL2_TTF_LIBRARY)
target_link_libraries(
    SnakeGame 
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARY}
)
# project path
target_compile_definitions(SnakeGame PRIVATE PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}") # add project path
