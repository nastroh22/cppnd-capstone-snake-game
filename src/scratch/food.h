#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <iostream>
#include "SDL.h"
#include "utils.h"


//TODO: change pattern so the name is the type (not really necessary to have its own
// then hard code paths in the constructor and use them once for instantiating

class Food {

public:

    Food(const std::string& name, const std::string& asset_file); // constructor
    // Implement Rule of Five on the Food Class
    virtual ~Food();                        // declare virtual destructor
    Food(const Food&) = delete;            // delete copy ctor
    Food& operator=(const Food&) = delete; // delete copy assign
    Food(Food&&) noexcept;                          // declare move ctor
    Food& operator=(Food&&) = delete;     // delete move assign

    // setters
    virtual void init_texture(SDL_Renderer* renderer) {
        sdl_texture=RenderUtils::InitTexture(renderer, _asset_file);
    }
    void set_position(int x, int y) { position.x = x; position.y = y; std::cout<< "What is the issue? " << std::endl; }
    
    //getters
    SDL_Point get_position() const { return position; }
    SDL_Texture *get_texture() const { return sdl_texture; }
    int get_x() const { return position.x; }
    int get_y() const { return position.y; }

    // info / debug
    void print_name() const { std::cout << "Food name: " << _name << std::endl; }
    void print_position() const { std::cout << "Food position: (" << position.x << ", " << position.y << ")\n";}


protected:
    std::string const _name;    // made this const 
    std::string const _asset_file; //NOTE: probably don't really need to hang onto this
    SDL_Texture *sdl_texture;
    SDL_Point position;
};

class Banana : public Food {

public:
    Banana() : Food("banana", "../assets/items/banana.bmp") {}
    ~Banana() override = default;

};

class Cherries : public Food {
public:
    Cherries() : Food("cherries", "../assets/items/cherry.bmp") {}
    ~Cherries() override = default;
};


class Star : public Food {
public:
    Star() : Food("star", "../assets/items/star.bmp") {}
    ~Star() override = default;
};

class Bomb : public Food {
public:
    Bomb() : Food("bomb", "../assets/items/bomb.bmp") {}
    ~Bomb() override = default;
};


class Dot : public Food {
public:
    Dot(bool is_blinking = false) : Food("dot", "../assets/items/dot.bmp"), _is_blinking(is_blinking) {}
    ~Dot() override = default;

private:
    bool const _is_blinking = false; // Example of additional attribute
};

#endif