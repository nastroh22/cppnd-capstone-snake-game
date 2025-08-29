#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <iostream>
#include "SDL.h"


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
    virtual void init_texture(SDL_Renderer* renderer);
    void set_position(int x, int y) { position.x = x; position.y = y; }
    
    //getter
    SDL_Point get_position() const { return position; }
    SDL_Texture *get_texture() const { return sdl_texture; }
    int get_x() const { return position.x; }
    int get_y() const { return position.y; }
    void print_name() const { std::cout << "Food name: " << _name << std::endl; }
    void print_position() const { std::cout << "Food position: (" << position.x << ", " << position.y << ")\n";}

    // std::string SetAssetFile(std::string file) {_asset_file = file;}; 
    //virtual Render() // NOTE come back to this, did have some concept of virtual "render" function
    //virtual PowerUp();
    // virtual void Render() const = 0; // TODO, eventually make Render "pure"
    //overload constructor with default empty asset file
    // Food(const std::string& name) : _name(name), _asset_file("") {}
    // std::string GetAssetFile() const { return _asset_file; }

    // virtual void Render() const {
    //     std::cout << "Rendering food: " << _name << " from asset file: " << _asset_file << std::endl;
    // }

protected:
    std::string const _name;    // made this const 
    std::string const _asset_file; //NOTE: probably don't really need to hang onto this
    SDL_Texture *sdl_texture;
    SDL_Point position;
};

class Banana : public Food {

public:
    Banana() : Food("banana", "../assets/pactiles/banana.bmp") {}
    ~Banana() override = default;

};

class Cherries : public Food {
public:
    Cherries() : Food("cherries", "../assets/pactiles/cherry.bmp") {}
    ~Cherries() override = default;
};

class Dot : public Food {
public:
    Dot(bool is_blinking = false) : Food("dot", "../assets/pactiles/dot.bmp"), _is_blinking(is_blinking) {}
    ~Dot() override = default;

private:
    bool const _is_blinking = false; // Example of additional attribute
};

#endif