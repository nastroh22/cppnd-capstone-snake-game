#include "food.h"
#include "utils.h"

// FUTURE TODO's:
// Add bomb (which removes part of snake body)
// Add powerup (which increases speed for a time? STAR = temporary invulnerable)
// Possible stepping stone for pacman as well would be giving snake's ability to "eat" enemies

//TODO: update this to get rid of const variables (use once)
Food::Food(const std::string& name, const std::string& asset_file) 
        : _name(name), _asset_file(asset_file), sdl_texture(nullptr), position{0, 0} {
        }  // base constructor

// Destructor
Food::~Food() {
    if (sdl_texture) SDL_DestroyTexture(sdl_texture);
}
//Move Construtor
Food::Food(Food&& other) noexcept
    : _name(std::move(other._name)),
      _asset_file(std::move(other._asset_file)),
      position(other.position),
      sdl_texture(other.sdl_texture) // shallo copy pointer
{
    other.sdl_texture = nullptr;
}
//others deleted
//Move Assign Deleted
// Food& Food::operator=(Food&& other) noexcept;
// if (this != &other) {
//         if (sdl_texture) SDL_DestroyTexture(sdl_texture); // free existing resource
// }

// TODO: possible render extension
// virtual void Render(SDL_Renderer* renderer) const {
//     if (sdl_texture) {
//         SDL_Rect dstRect{position.x, position.y, width, height}; // You'll need width/height
//         SDL_RenderCopy(renderer, sdl_texture, nullptr, &dstRect);
//     }
// }