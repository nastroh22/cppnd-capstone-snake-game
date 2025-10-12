cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


# target
add_executable(SnakeGame src/main.cpp src/game.cpp src/controller.cpp src/renderer.cpp
                    src/snake.cpp src/menu/components.cpp src/menu/menu.cpp src/enemy/planner.cpp)
                      
## Optional, add platform suffix to the target name
# if(WIN32)
# set(target_name "${target_name}_WIN")
# elseif(APPLE)
#     set(target_name "${target_name}_MAC")
# endif()

# set paths
if(WIN32)
    # Look in external 
    list(APPEND CMAKE_PREFIX_PATH
        "${CMAKE_SOURCE_DIR}/external/SDL2/lib/cmake"
        "${CMAKE_SOURCE_DIR}/external/SDL2/lib/"
        "${CMAKE_SOURCE_DIR}/external/SDL2_ttf/lib/cmake"
        "${CMAKE_SOURCE_DIR}/external/SDL2_ttf/lib/"
        # "${CMAKE_SOURCE_DIR}/external/SDL2_ttf/lib/pkgconfig"
        # "${CMAKE_SOURCE_DIR}/external/SDL2_ttf/lib/cmake/SDL2_ttf"
    )
    message(STATUS "Targeting WIN32, CMAKE_PREFIX=${CMAKE_PREFIX_PATH}")

elseif(APPLE)
    # Look in homebrew
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/lib/cmake/")
    message(STATUS "Targeting APPLE, CMAKE_PREFIX=${CMAKE_PREFIX_PATH}")
#else() no other target platforms
endif()

message(STATUS "Again, CMAKE_PREFIX=${CMAKE_PREFIX_PATH}")
# find
find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)


# link
target_link_libraries(SnakeGame PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf Threads::Threads) #SDL2::SDL2main
include_directories(src)

# MinGw specific settings
# if (MINGW)
#     target_link_options(SnakeGame APPEND PRIVATE "-mconsole")
#     message(STATUS "Check MINGW Flag = ${MINGW}")
# endif()

## Need to override "WinMain" expectation with MinGw
if (MINGW)
    get_target_property(existing_flags SnakeGame LINK_FLAGS)
    if (NOT existing_flags)
        set(existing_flags "")
        message(STATUS "No existing flags?")
    endif()

    set_target_properties(SnakeGame PROPERTIES
        LINK_FLAGS "${existing_flags} -mconsole"
    )
    message(STATUS "Exisitng Flags? ${existing_flags} ${LINK_FLAGS}" )
endif()


#debug paths
if(TARGET SDL2_ttf::SDL2_ttf)
    get_target_property(sdl2ttf_inc SDL2_ttf::SDL2_ttf INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "SDL2_ttf include directories: ${sdl2ttf_inc}")
else()
    message(WARNING "SDL2_ttf::SDL2_ttf target not found!")
endif()
message(STATUS "SDL2_ttf library    : ${SDL2_TTF_LIBRARY}")

#debug paths
message(STATUS "SDL2 include dir    : ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 library        : ${SDL2_LIBRARIES}")

# debug target properties
get_target_property(opts SnakeGame LINK_OPTIONS)
message(STATUS "Link options for SnakeGame: ${opts}")


# add project root to target
target_compile_definitions(SnakeGame PRIVATE PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}") # add project path



# scratch (todo)
# include_directories(${SDL2_INCLUDE_DIRS} src)
# # string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
# # string(STRIP ${SDL2_TTF_LIBRARY} SDL2_TTF_LIBRARY)
# target_link_libraries(SnakeGame PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf)