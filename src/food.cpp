#include "food.h"


// fancier way, smart pointer with custom deleter
// struct SDLTextureDeleter {
//     void operator()(SDL_Texture* texture) const {
//         if (texture) SDL_DestroyTexture(texture);
//     }
// };

//TODO: update this to get rid of const variables (use once)
Food::Food(const std::string& name, const std::string& asset_file) 
        : _name(name), _asset_file(asset_file), sdl_texture(nullptr), position{0, 0} {}  // base constructor

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

void Food::init_texture(SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(_asset_file.c_str());
    if (!surface) {
        std::cerr << "Failed to load BMP: " << SDL_GetError() << std::endl;
        sdl_texture = nullptr; // Handle error or set to nullptr
    } else {
        sdl_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!sdl_texture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            // Handle error or set to nullptr
        }
    }
}

// TODO: possible render extension
// virtual void Render(SDL_Renderer* renderer) const {
//     if (sdl_texture) {
//         SDL_Rect dstRect{position.x, position.y, width, height}; // You'll need width/height
//         SDL_RenderCopy(renderer, sdl_texture, nullptr, &dstRect);
//     }
// }