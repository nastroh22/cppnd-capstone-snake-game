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


## Resources:
- [cool free fonts](https://www.fontspace.com/category/arcade)
- [sprite files](https://opengameart.org/)
- [color palettes](https://colorswall.com/palette/3036)


## useful command:

- default search paths: `clang++ -v -E - < /dev/null`

- "search starts here"
```
/Library/Developer/CommandLineTools/usr/lib/clang/16/include
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include
/Library/Developer/CommandLineTools/usr/include
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks
```

# New Features
My implementation of the snake game extends the base project in the following ways:
- Inclusion of game menu that provides various options for the user to interact with, such as viewing high scores, choosing a character icon to play as, naming their character, and quitting the menu.
- The control flow structure of the game has also been updated so that at the conclusion of a single round, instead of quitting, the game returns to the main menu, allowing the plauyer to choose to play again.
- Addition of "enemies" into the game which include a pursuer, a guard and moving obstacles. These are launched as new threads that concurrently plan their moves during game play.
- Addition of different "food" objects, which currently simply load different icons randomly, but with plans in the future to tie the items to different rules, power-ups, score boosts, etc.
- Concurrency relies on the use of several message queus for publishing and receiving player/enemy positions where the queue follows the basic structure from module 4 of this course



# Rubric Criteria

##  Loops, Functions, I/O
| Criteria         | Project Feature                    |
| ---------------- | -------------------------------------- | 
| The project demonstrates an understanding of C++ functions and control structures.  | <ul><li>Game state managed through case/switch control structure. Which defines when to load menus. </li> <li> Planner thread is carefully controlled through std::atomic shutdown flag </li> <li> Ample use of functions to encapsulate and control pursuer and menu logic </li></ul> |
| The project reads data from a file and process the data, or the program writes data to a file.  | <ul><li>Loads and renders .bmp files for characters and food items </li> <li>Uses std::filesystem to save/load high scores </li> </ul> |
| The project accepts user input and processes the input. |<ul><li>Player name is accepted as user input </li> <li>Input is processed by associating name to score</li> </ul> |
| The project uses data structures and immuatable variables |<ul><li> Use of both arrays (for finite list of items) and vectors (in message queues) </li> <li>Const variables are frequently defined in new classes often for immutable pointers or rendering constants (colors, rectangle sizes) which can be defined by the game designer </li> </ul> |


##  Object-Oriented Programming
| Criteria         | Project Feature                    |
| ---------------- | -------------------------------------- | 
| One or more classes are added to the project with appropriate access specifiers for class members.  | <ul><li> Menu is implemented as hierarchical class structure </li> <li> Food items are also new classes </li> <li> Planner is a new class ( and character inheritance TODO) </li> <li> All of these are accessed appropriately throughout game control strcuture </li></ul> |
| Class constructors utilize member initialization lists. | <ul><li> Menu classes frequently use initializer lists to set rendering constants </li> |
| Classes abstract implementation details from their interfaces. |<ul><li> Menu class for example is fully abstracted, such that the main game loop simply uses a "MenuManager.run()" call which then handles all menu loading/switching logic internally <li> The menu design is also fully customizable where, upon object construction, desgin choices are automatically applied through the simple setting of constants before compiling </li></li>|
| Overloaded functions allow the same function to operate on different parameters. |<ul><li> (Possibly) enemy "run" target is overloaded </li> <li>While not technically "overloaded" the menu "Render()" calls are custom to the element they render </li> <li>Otherwise, I don't think I use any overloading</li> </ul> |
| Classes follow an appropriate inheritance hierarchy with virtual and override functions. |<ul><li> Menu and Food are both defined with a clear inheritance structure </li> <li> For example, a menu object has sub-elements like "Window" or "Button" which inherit from their parent container and extend with specific colors/shapes etc. </li> </ul> |
| Templates generalize functions or classes in the project.|<ul><li> Templates are used in the MessageQueue class </li> |


##  Memory Management
| Criteria         | Project Feature                    |
| ---------------- | -------------------------------------- | 
| The project makes use of references in function declarations.  | <ul><li> References are added to the `game.run()` function to aid in state management so that there is a single instance of "Planner" for example </li> Other <li> </li> |
| The project uses destructors appropriately. | <ul><li> Most destructors are allowed to be default due to RAII and smart pointers </li> <li> However, the destructor of the "Text" object in particular uses "delete" to free resources and is cleaned up upon destruction of the menu object </li> |
| The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate. | <ul><li> Scope and RAII are heavily depended upon for the `Planner` and `Menu` objects </li> |
| The project follows the Rule of 5 | <ul><li> Defaults are mostly used </li> <li> However, the food class structure uses rule of 5, albeit it is through deletion of certain operations not needed such as copy construct and copy move operator </li>|
| The project uses move semantics to move data instead of copying it, where possible. | <ul><li> Move semantics are utilized in the `MessageQueue` </li> |
| The project uses smart pointers instead of raw pointers. | <ul><li> Shared pointers are used for queues, so that both main and planner threads have shared access while lifetime management is automated </li> <li> Food object uses unique_pointer since ... </li> |


##  Concurrency
| Criteria         | Project Feature                    |
| ---------------- | -------------------------------------- | 
| The project uses multithreading. | <ul><li> Planner and main threads run concurrently </li> |
| A mutex or lock is used in the project. | <ul><li> MessageQueue uses a unique_lock and lock guard as outlined in the pattern of module 4 in this course </li> |
| A condition variable is used in the project. | <ul><li> Again, utilized in the `MessageQueue` </li> |


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



