## Cmake:
- https://cmake.org/getting-started/
- https://www.willusher.io/sdl2%20tutorials/2014/03/06/lesson-0-cmake/


- basic minimal CMakeLists.txt example:
    - cmake_minimum_required(VERSION 3.7)


## clang
- can check default version of compiler with: 
    -   (i have no idea what all this hoodoo black magic means)

- can set an alias to use c++17 standard as so:
    - `alias clang++="clang++ -std=c++17"`

- additionally set c++ standard in `CMakeLists.txt` like so:
    - set(CMAKE_CXX_STANDARD 17)
    - set(CMAKE_CXX_STANDARD_REQUIRED ON)
    - set(CMAKE_CXX_EXTENSIONS OFF)



###  install cmake for macOS

`brew install cmake`
* binary to -> `/opt/homebrew/bin/cmake`
* versions in -> `/opt/homebrew/Cellar/cmake/4.1.0`


### install make for macOS
`brew install make`
* needed upgraded version (>= 4.1)
* brew installs "gnu make" as `gmake`
* `which make` -> `usr/bin/make`
* `which gmake` -> `opt/homebrew/bin/gmake`

* `brew install make` does several things:
    - creates a symlink to `gmake` located at opt/homebrew/bin -> package binary in /opt/homebrew/Cellar
    - creates a symlink for `make` located at opt/homebew/opt/make
    - if you want to use command `make` you can add this latter symlink to PATH with `PATH="/opt/homebrew/opt/make/libexec/gnubin:$PATH"`


## install SDL2

`brew install sdl2`
* to find where library files are installed on your system run `brew list sdl2`
* (likely in /opt/homebrew/Cellar)
* Symlinks created atL
    - /opt/homebrew/include/SDL2 (header files)
    - 
* `sdl2-config` to get compiler flags


For instructions on building from source see `https://wiki.libsdl.org/SDL2/Installation`


- add these lines to CMAKE:
```find_package(SDL2 REQUIRED)
message(STATUS "SDL2_INCLUDE_DIRS = ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES = ${SDL2_LIBRARIES}")
message(STATUS "SDL2_FOUND = ${SDL2_FOUND}")
```



## SDL:
https://wiki.libsdl.org/SDL3/APIByCategory

## useful command:

- default search paths: `clang++ -v -E - < /dev/null`

- "search starts here"
```
/Library/Developer/CommandLineTools/usr/lib/clang/16/include
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include
/Library/Developer/CommandLineTools/usr/include
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks
```



# Rubric Items & New Features:

## OOP
Mostly practicing oop concepts with the `Food` abstract type and it's derived classes.

- Polymorphism:
    - Each `Food` type has custom `Consume` behavior, for example:
        - Banana is worth +10 (and expires faster)
        - Cherries are worth +5
        - Blinking Dot's switch game state to "power up mode" 

    - Constructors

- Memory Management 
    - Map object has ownership of Foods (which are inherently tied to the position in pacman)
    - `.erase` cleans up resources automatically by defining foods as a vector of unique_pointers, calling the destructor on the smart pointer, hence, on the object itself.

Implementation of Food Class checks off following Rubric items:
    - [ ] *polymorphism*
    - [ ] *something else*


## Next Feature: Menu

- Menu will display high scores (possible support scrolling)
    

- Menu will have a "play" button

- Menu will have an exit button

- Character options (see next feature)



## Next Feature: Character Sprites
- different head designs (maybe use nintendo characters idk)
- different colors


## Next Feature: 



## Snake Game Rule Modifications:

Let's make it more interesting with different foods

- bomb: instant loss
- star: like super mario, invincibility against ghost (shrink snake maybe)
    - moving target that times out, if you get it the whole field populates with food
    - you get more speed and you're allowed to phase through walls (which is already allowed but thinking to change that)

- green potion: shrink snake without affecting score
- blue potion: spawns a new ghost

- update rules so that we can't move through boundaries?




## Notes on Next Steps (PacMan)

- I/O : map parser (likely as part of Map Object which also handles the above food layout)
    - Map Object will also be responsible for communicating with the game AI for defining where to place barriers etc. necessary for A*



